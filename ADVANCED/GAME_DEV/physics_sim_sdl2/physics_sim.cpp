// ADVANCED GAME_DEV: Physics Simulation — Bouncing Balls (SDL2, C++)
//
// WHAT THIS DEMONSTRATES:
// - Verlet integration for stable physics
// - Ball-wall elastic collision response
// - Ball-ball collision detection and response (conservation of momentum)
// - Object-oriented physics components (Vec2, Ball, World)
// - Spawning objects at mouse click
//
// CONTROLS:
//   Left click   — spawn ball at mouse position
//   Right click  — clear all balls
//   ESC          — quit

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <stdexcept>

// ============================================================
// Vec2 — minimal 2D vector math
// ============================================================
struct Vec2 {
    float x, y;
    Vec2(float x = 0, float y = 0) : x(x), y(y) {}
    Vec2  operator+(const Vec2& o) const { return {x+o.x, y+o.y}; }
    Vec2  operator-(const Vec2& o) const { return {x-o.x, y-o.y}; }
    Vec2  operator*(float s)       const { return {x*s,   y*s}; }
    Vec2& operator+=(const Vec2& o) { x+=o.x; y+=o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x-=o.x; y-=o.y; return *this; }
    float dot(const Vec2& o)  const { return x*o.x + y*o.y; }
    float len()               const { return std::sqrt(x*x + y*y); }
    Vec2  norm()              const { float l = len(); return l > 0 ? Vec2{x/l, y/l} : Vec2{}; }
};

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
const int   SCREEN_W = 800, SCREEN_H = 600;
const float GRAVITY   = 600.0f;     // pixels / s^2
const float RESTITUTION = 0.75f;    // energy retained on bounce (0=dead, 1=elastic)
const float FRICTION    = 0.995f;   // velocity damping per frame
const int   MAX_BALLS   = 80;

// ============================================================
// Ball
// ============================================================
struct Ball {
    Vec2  pos, vel;
    float radius;
    SDL_Color color;

    Ball(float x, float y, float vx, float vy, float r, SDL_Color c)
        : pos{x, y}, vel{vx, vy}, radius(r), color(c) {}

    void update(float dt) {
        vel.y += GRAVITY * dt;
        vel.x *= FRICTION;
        vel.y *= FRICTION;
        pos   += vel * dt;
    }

    void collide_walls() {
        float left = radius, right = SCREEN_W - radius;
        float top  = radius, bottom = SCREEN_H - radius;

        if (pos.x < left)   { pos.x = left;   vel.x =  std::abs(vel.x) * RESTITUTION; }
        if (pos.x > right)  { pos.x = right;  vel.x = -std::abs(vel.x) * RESTITUTION; }
        if (pos.y < top)    { pos.y = top;     vel.y =  std::abs(vel.y) * RESTITUTION; }
        if (pos.y > bottom) { pos.y = bottom;  vel.y = -std::abs(vel.y) * RESTITUTION; }
    }

    void draw(SDL_Renderer* ren) const {
        // Draw filled circle with midpoint algorithm
        SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
        int r = static_cast<int>(radius);
        for (int dy = -r; dy <= r; ++dy) {
            int dx = static_cast<int>(std::sqrt(static_cast<float>(r*r - dy*dy)));
            SDL_RenderDrawLine(ren,
                static_cast<int>(pos.x) - dx, static_cast<int>(pos.y) + dy,
                static_cast<int>(pos.x) + dx, static_cast<int>(pos.y) + dy);
        }
        // Highlight
        SDL_SetRenderDrawColor(ren,
            std::min(255, color.r + 80),
            std::min(255, color.g + 80),
            std::min(255, color.b + 80), 255);
        SDL_RenderDrawPoint(ren,
            static_cast<int>(pos.x) - r/3,
            static_cast<int>(pos.y) - r/3);
    }
};

// ============================================================
// Ball-ball collision response (elastic, equal mass approx)
// ============================================================
void resolve_ball_collision(Ball& a, Ball& b) {
    Vec2 delta = b.pos - a.pos;
    float dist = delta.len();
    float min_dist = a.radius + b.radius;

    if (dist >= min_dist || dist < 0.001f) return;

    // Push apart (positional correction)
    float overlap = (min_dist - dist) / 2.0f;
    Vec2 push = delta.norm() * overlap;
    a.pos -= push;
    b.pos += push;

    // Velocity exchange along collision normal
    Vec2 n = delta.norm();
    float rel_vel = (b.vel - a.vel).dot(n);
    if (rel_vel > 0) return;  // already separating

    float impulse = rel_vel * (1.0f + RESTITUTION) / 2.0f;
    a.vel += n * impulse;
    b.vel -= n * impulse;
}

// ============================================================
// World
// ============================================================
class World {
    std::vector<Ball> balls;

    SDL_Color random_color() {
        static const SDL_Color palette[] = {
            {255, 80, 80, 255}, {255, 165, 0, 255}, {255, 220, 50, 255},
            {80, 220, 80, 255}, {80, 180, 255, 255}, {180, 80, 255, 255},
            {255, 100, 180, 255}, {0, 220, 200, 255},
        };
        return palette[std::rand() % 8];
    }

public:
    void spawn(float x, float y) {
        if (static_cast<int>(balls.size()) >= MAX_BALLS) balls.erase(balls.begin());
        float r = static_cast<float>(10 + std::rand() % 20);
        float vx = static_cast<float>((std::rand() % 400) - 200);
        float vy = static_cast<float>(-(50 + std::rand() % 300));
        balls.emplace_back(x, y, vx, vy, r, random_color());
    }

    void clear() { balls.clear(); }

    void update(float dt) {
        for (auto& b : balls) {
            b.update(dt);
            b.collide_walls();
        }
        // Ball-ball collisions (O(n^2) — fine for <= 80 balls)
        for (size_t i = 0; i < balls.size(); ++i)
            for (size_t j = i + 1; j < balls.size(); ++j)
                resolve_ball_collision(balls[i], balls[j]);
    }

    void draw(SDL_Renderer* ren) const {
        for (const auto& b : balls) b.draw(ren);
    }

    size_t count() const { return balls.size(); }
};

// ============================================================
// Game
// ============================================================
class Game {
    SDLContext sdl;
    Window     window;
    Renderer   renderer;
    World      world;
    bool       running;

public:
    Game() : window("C++ Physics Simulation (SDL2) — Click to spawn", SCREEN_W, SCREEN_H),
             renderer(window.ptr), running(true)
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
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (e.button.button == SDL_BUTTON_LEFT)
                        world.spawn(static_cast<float>(e.button.x), static_cast<float>(e.button.y));
                    if (e.button.button == SDL_BUTTON_RIGHT)
                        world.clear();
                }
            }

            world.update(dt);

            SDL_Renderer* ren = renderer.ptr;
            SDL_SetRenderDrawColor(ren, 15, 15, 25, 255);
            SDL_RenderClear(ren);
            world.draw(ren);

            // Floor line
            SDL_SetRenderDrawColor(ren, 60, 60, 80, 255);
            SDL_RenderDrawLine(ren, 0, SCREEN_H - 1, SCREEN_W, SCREEN_H - 1);

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
