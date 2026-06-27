// ADVANCED GAME_DEV: Particle System with Object Pool (SDL2, C++)
//
// WHAT THIS DEMONSTRATES:
// - Object pool pattern: pre-allocated fixed array, reuse slots (no new/delete per frame)
// - Particle emitters with configurable burst/continuous modes
// - Alpha fade and size shrink over lifetime
// - Multiple emitter types selectable at runtime
//
// CONTROLS:
//   1-4     — switch emitter type
//   Click   — move emitter to mouse
//   ESC     — quit

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <array>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <string>

// ============================================================
// SDL2 RAII
// ============================================================
struct SDLContext {
    SDLContext() { if (SDL_Init(SDL_INIT_VIDEO) < 0) throw std::runtime_error(SDL_GetError()); }
    ~SDLContext() { SDL_Quit(); }
};
struct Window {
    SDL_Window* ptr;
    Window(const char* t, int w, int h)
        : ptr(SDL_CreateWindow(t, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0))
    { if (!ptr) throw std::runtime_error(SDL_GetError()); }
    ~Window() { SDL_DestroyWindow(ptr); }
};
struct Renderer {
    SDL_Renderer* ptr;
    Renderer(SDL_Window* w)
        : ptr(SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
    { if (!ptr) throw std::runtime_error(SDL_GetError()); }
    ~Renderer() { SDL_DestroyRenderer(ptr); }
};

// ============================================================
// Constants
// ============================================================
const int SCREEN_W = 800, SCREEN_H = 600;
const int POOL_SIZE = 2000;  // max simultaneous particles

// ============================================================
// Particle (plain data — no vtable, tight in memory)
// ============================================================
struct Particle {
    float x, y;
    float vx, vy;
    float life;       // remaining life in seconds
    float max_life;
    float size;       // starting radius
    Uint8 r, g, b;
    bool  active;
};

// ============================================================
// Object Pool — pre-allocated, O(1) alloc/free
// ============================================================
class ParticlePool {
    std::array<Particle, POOL_SIZE> pool{};
    int next_free{0};   // round-robin search start

public:
    Particle* alloc() {
        // Search for an inactive slot starting at next_free
        for (int i = 0; i < POOL_SIZE; ++i) {
            int idx = (next_free + i) % POOL_SIZE;
            if (!pool[idx].active) {
                next_free = (idx + 1) % POOL_SIZE;
                pool[idx].active = true;
                return &pool[idx];
            }
        }
        // Pool full: reclaim oldest (first in array that's active) — graceful degradation
        pool[next_free].active = true;
        return &pool[next_free++];
    }

    void update(float dt) {
        for (auto& p : pool) {
            if (!p.active) continue;
            p.vx *= 0.98f;
            p.vy *= 0.98f;
            p.vy += 50.0f * dt;    // light gravity
            p.x  += p.vx * dt;
            p.y  += p.vy * dt;
            p.life -= dt;
            if (p.life <= 0) p.active = false;
        }
    }

    void draw(SDL_Renderer* ren) const {
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_ADD);
        for (const auto& p : pool) {
            if (!p.active) continue;
            float t = p.life / p.max_life;   // 1 = fresh, 0 = dying
            Uint8 alpha = static_cast<Uint8>(t * 220);
            int sz = std::max(1, static_cast<int>(p.size * t));
            SDL_SetRenderDrawColor(ren, p.r, p.g, p.b, alpha);
            SDL_Rect r{
                static_cast<int>(p.x) - sz/2,
                static_cast<int>(p.y) - sz/2,
                sz, sz
            };
            SDL_RenderFillRect(ren, &r);
        }
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
    }

    int active_count() const {
        int c = 0;
        for (const auto& p : pool) if (p.active) ++c;
        return c;
    }
};

// ============================================================
// Emitter types
// ============================================================
enum class EmitterType { FIRE=1, EXPLOSION=2, SNOW=3, FOUNTAIN=4 };

static float randf(float lo, float hi) {
    return lo + (hi - lo) * (std::rand() / static_cast<float>(RAND_MAX));
}

void emit_fire(ParticlePool& pool, float cx, float cy, float dt) {
    int count = static_cast<int>(80 * dt) + (std::rand() % 3);
    for (int i = 0; i < count; ++i) {
        Particle* p = pool.alloc();
        p->x = cx + randf(-15, 15);
        p->y = cy + randf(-5, 5);
        p->vx = randf(-30, 30);
        p->vy = randf(-200, -80);
        p->life = p->max_life = randf(0.4f, 1.2f);
        p->size = randf(4, 10);
        p->r = 255; p->g = static_cast<Uint8>(randf(60, 160)); p->b = 0;
    }
}

void emit_explosion(ParticlePool& pool, float cx, float cy) {
    for (int i = 0; i < 300; ++i) {
        Particle* p = pool.alloc();
        float angle = randf(0, 2 * 3.14159f);
        float speed = randf(50, 500);
        p->x = cx; p->y = cy;
        p->vx = std::cos(angle) * speed;
        p->vy = std::sin(angle) * speed;
        p->life = p->max_life = randf(0.3f, 1.5f);
        p->size = randf(3, 8);
        Uint8 choice = std::rand() % 3;
        if (choice == 0) { p->r = 255; p->g = 200; p->b = 50; }
        else if (choice == 1) { p->r = 255; p->g = 80; p->b = 0; }
        else { p->r = 200; p->g = 200; p->b = 200; }
    }
}

void emit_snow(ParticlePool& pool, float dt) {
    int count = static_cast<int>(30 * dt) + 1;
    for (int i = 0; i < count; ++i) {
        Particle* p = pool.alloc();
        p->x = randf(0, SCREEN_W);
        p->y = -5;
        p->vx = randf(-20, 20);
        p->vy = randf(30, 80);
        p->life = p->max_life = randf(3.0f, 8.0f);
        p->size = randf(2, 5);
        p->r = p->g = p->b = 220;
    }
}

void emit_fountain(ParticlePool& pool, float cx, float cy, float dt) {
    int count = static_cast<int>(100 * dt) + 1;
    for (int i = 0; i < count; ++i) {
        Particle* p = pool.alloc();
        p->x = cx + randf(-5, 5);
        p->y = cy;
        float angle = randf(-3.14159f * 0.6f, -3.14159f * 0.4f);
        float speed = randf(150, 350);
        p->vx = std::cos(angle) * speed + randf(-20, 20);
        p->vy = std::sin(angle) * speed;
        p->life = p->max_life = randf(1.0f, 2.5f);
        p->size = randf(3, 7);
        p->r = 80; p->g = 180; p->b = 255;
    }
}

// ============================================================
// Game
// ============================================================
class Game {
    SDLContext sdl;
    Window     window;
    Renderer   renderer;
    ParticlePool pool;

    bool running{true};
    float emitter_x{SCREEN_W / 2.0f};
    float emitter_y{SCREEN_H / 2.0f};
    EmitterType mode{EmitterType::FIRE};
    bool explosion_triggered{false};

public:
    Game() : window("C++ Particle System — Object Pool (SDL2) — 1:Fire 2:Explode 3:Snow 4:Fountain",
                    SCREEN_W, SCREEN_H),
             renderer(window.ptr)
    {
        std::srand(static_cast<unsigned>(SDL_GetTicks()));
    }

    void run() {
        Uint32 last_ticks = SDL_GetTicks();

        while (running) {
            Uint32 now = SDL_GetTicks();
            float dt = std::min((now - last_ticks) / 1000.0f, 0.033f);
            last_ticks = now;

            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) running = false;
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE: running = false; break;
                        case SDLK_1: mode = EmitterType::FIRE; break;
                        case SDLK_2: mode = EmitterType::EXPLOSION; explosion_triggered = true; break;
                        case SDLK_3: mode = EmitterType::SNOW; break;
                        case SDLK_4: mode = EmitterType::FOUNTAIN; break;
                    }
                }
                if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION) {
                    if (e.type == SDL_MOUSEBUTTONDOWN || SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(1)) {
                        int mx, my;
                        SDL_GetMouseState(&mx, &my);
                        emitter_x = static_cast<float>(mx);
                        emitter_y = static_cast<float>(my);
                        if (mode == EmitterType::EXPLOSION) explosion_triggered = true;
                    }
                }
            }

            // Emit
            switch (mode) {
                case EmitterType::FIRE:
                    emit_fire(pool, emitter_x, emitter_y, dt);
                    break;
                case EmitterType::EXPLOSION:
                    if (explosion_triggered) {
                        emit_explosion(pool, emitter_x, emitter_y);
                        explosion_triggered = false;
                    }
                    break;
                case EmitterType::SNOW:
                    emit_snow(pool, dt);
                    break;
                case EmitterType::FOUNTAIN:
                    emit_fountain(pool, emitter_x, emitter_y, dt);
                    break;
            }

            pool.update(dt);

            // Render
            SDL_Renderer* ren = renderer.ptr;
            SDL_SetRenderDrawColor(ren, 8, 8, 12, 255);
            SDL_RenderClear(ren);
            pool.draw(ren);

            // HUD: show active count and mode
            SDL_SetRenderDrawColor(ren, 60, 60, 80, 255);
            // Draw particle count as a simple bar
            int bar = pool.active_count() * SCREEN_W / POOL_SIZE;
            SDL_Rect br{0, SCREEN_H - 4, bar, 4};
            SDL_SetRenderDrawColor(ren, 100, 200, 100, 255);
            SDL_RenderFillRect(ren, &br);

            SDL_RenderPresent(ren);
        }
    }
};

int main(int, char**) {
    try {
        Game g;
        g.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
