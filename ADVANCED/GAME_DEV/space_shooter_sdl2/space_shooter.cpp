// ADVANCED GAME_DEV: 2D Space Shooter (SDL2, C++ OOP)
//
// WHAT THIS DEMONSTRATES:
// - Entity component pattern (base Entity class with Player/Enemy/Bullet derived)
// - Object pooling awareness (vector of active entities)
// - Collision detection (AABB)
// - Game loop with fixed-step update + render
// - RAII for SDL resources
//
// CONTROLS: Arrow keys / WASD to move, SPACE to shoot, ESC to quit

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <cmath>

// ============================================================
// SDL2 RAII wrappers
// ============================================================
struct SDLContext {
    SDLContext() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
        }
    }
    ~SDLContext() { SDL_Quit(); }
    SDLContext(const SDLContext&) = delete;
};

struct Window {
    SDL_Window* ptr;
    Window(const char* title, int w, int h)
        : ptr(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0))
    {
        if (!ptr) throw std::runtime_error(std::string("CreateWindow: ") + SDL_GetError());
    }
    ~Window() { if (ptr) SDL_DestroyWindow(ptr); }
};

struct Renderer {
    SDL_Renderer* ptr;
    Renderer(SDL_Window* win)
        : ptr(SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
    {
        if (!ptr) throw std::runtime_error(std::string("CreateRenderer: ") + SDL_GetError());
    }
    ~Renderer() { if (ptr) SDL_DestroyRenderer(ptr); }
};

// ============================================================
// Constants
// ============================================================
const int SCREEN_W = 800, SCREEN_H = 600;
const float PLAYER_SPEED = 250.0f;
const float BULLET_SPEED = 500.0f;
const float ENEMY_SPEED  = 80.0f;

// ============================================================
// Base Entity
// ============================================================
class Entity {
public:
    float x, y, w, h;
    bool  active;
    SDL_Color color;

    Entity(float x, float y, float w, float h, SDL_Color col)
        : x(x), y(y), w(w), h(h), active(true), color(col) {}

    virtual ~Entity() {}
    virtual void update(float dt) = 0;

    void draw(SDL_Renderer* ren) const {
        if (!active) return;
        SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
        SDL_FRect rect{x - w/2, y - h/2, w, h};
        SDL_RenderFillRectF(ren, &rect);
    }

    bool collides(const Entity& other) const {
        if (!active || !other.active) return false;
        return std::abs(x - other.x) < (w + other.w)/2 &&
               std::abs(y - other.y) < (h + other.h)/2;
    }
};

// ============================================================
// Bullet
// ============================================================
class Bullet : public Entity {
    float vy;
public:
    Bullet(float x, float y, float vy)
        : Entity(x, y, 4, 12, {255, 255, 0, 255}), vy(vy) {}

    void update(float dt) override {
        y += vy * dt;
        if (y < -20 || y > SCREEN_H + 20) active = false;
    }
};

// ============================================================
// Player
// ============================================================
class Player : public Entity {
public:
    int score;
    int lives;

    Player()
        : Entity(SCREEN_W/2.0f, SCREEN_H - 60.0f, 36, 28, {0, 200, 255, 255}),
          score(0), lives(3) {}

    void update(float dt) override {
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        if ((keys[SDL_SCANCODE_LEFT]  || keys[SDL_SCANCODE_A]) && x > w/2)
            x -= PLAYER_SPEED * dt;
        if ((keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) && x < SCREEN_W - w/2)
            x += PLAYER_SPEED * dt;
        if ((keys[SDL_SCANCODE_UP]    || keys[SDL_SCANCODE_W]) && y > h/2)
            y -= PLAYER_SPEED * dt;
        if ((keys[SDL_SCANCODE_DOWN]  || keys[SDL_SCANCODE_S]) && y < SCREEN_H - h/2)
            y += PLAYER_SPEED * dt;
    }

    bool can_shoot() {
        static Uint32 last_shot = 0;
        Uint32 now = SDL_GetTicks();
        if (now - last_shot > 250) { last_shot = now; return true; }
        return false;
    }
};

// ============================================================
// Enemy
// ============================================================
class Enemy : public Entity {
    float vx;
    float spawn_y;
    float wobble;
public:
    Enemy(float x, float y)
        : Entity(x, y, 32, 24, {255, 50, 50, 255}),
          vx(((std::rand() % 2) * 2 - 1) * ENEMY_SPEED),
          spawn_y(y), wobble(static_cast<float>(std::rand() % 314) / 100.0f)
    {}

    void update(float dt) override {
        x += vx * dt;
        wobble += dt * 2.0f;
        y = spawn_y + std::sin(wobble) * 30.0f;
        if (x < w/2 || x > SCREEN_W - w/2) vx = -vx;
        spawn_y += 15.0f * dt;
        if (spawn_y > SCREEN_H + 50) active = false;
    }
};

// ============================================================
// Game
// ============================================================
class Game {
    SDLContext sdl;
    Window     window;
    Renderer   renderer;

    Player                        player;
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Enemy>>  enemies;

    bool running;
    float enemy_spawn_timer;
    Uint32 last_ticks;

    void spawn_enemy() {
        float ex = static_cast<float>(30 + std::rand() % (SCREEN_W - 60));
        enemies.push_back(std::make_unique<Enemy>(ex, -30.0f));
    }

    void clear_inactive() {
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const auto& b){ return !b->active; }), bullets.end());
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const auto& e){ return !e->active; }), enemies.end());
    }

    void check_collisions() {
        for (auto& bullet : bullets) {
            for (auto& enemy : enemies) {
                if (bullet->active && enemy->active && bullet->collides(*enemy)) {
                    bullet->active = false;
                    enemy->active  = false;
                    player.score  += 100;
                }
            }
        }
        for (auto& enemy : enemies) {
            if (enemy->active && player.active && enemy->collides(player)) {
                enemy->active = false;
                --player.lives;
                if (player.lives <= 0) player.active = false;
            }
        }
    }

    void draw_hud() {
        // Minimal HUD using colored rectangles as health bars
        SDL_Renderer* ren = renderer.ptr;
        // Lives bar
        for (int i = 0; i < player.lives; ++i) {
            SDL_SetRenderDrawColor(ren, 0, 200, 255, 255);
            SDL_Rect r{10 + i * 20, 10, 14, 14};
            SDL_RenderFillRect(ren, &r);
        }
        // Score bar (proportional width)
        int sw = std::min(player.score / 10, SCREEN_W - 60);
        SDL_SetRenderDrawColor(ren, 255, 200, 0, 255);
        SDL_Rect sb{(SCREEN_W - sw) / 2, 10, sw, 8};
        SDL_RenderFillRect(ren, &sb);
    }

public:
    Game() : window("C++ Space Shooter (SDL2)", SCREEN_W, SCREEN_H),
             renderer(window.ptr), running(true),
             enemy_spawn_timer(0), last_ticks(SDL_GetTicks())
    {
        std::srand(static_cast<unsigned>(SDL_GetTicks()));
    }

    void run() {
        while (running) {
            Uint32 now = SDL_GetTicks();
            float dt = (now - last_ticks) / 1000.0f;
            last_ticks = now;
            dt = std::min(dt, 0.05f);  // cap delta time

            // Events
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) running = false;
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE && player.active) {
                    if (player.can_shoot()) {
                        bullets.push_back(std::make_unique<Bullet>(player.x, player.y - 20, -BULLET_SPEED));
                    }
                }
            }

            // Update
            if (player.active) player.update(dt);
            for (auto& b : bullets) b->update(dt);
            for (auto& en : enemies) en->update(dt);

            enemy_spawn_timer += dt;
            if (enemy_spawn_timer > 1.2f) {
                spawn_enemy();
                enemy_spawn_timer = 0;
            }

            check_collisions();
            clear_inactive();

            // Game over
            if (!player.active) {
                std::cout << "GAME OVER! Score: " << player.score << "\n";
                SDL_Delay(2000);
                running = false;
            }

            // Render
            SDL_Renderer* ren = renderer.ptr;
            SDL_SetRenderDrawColor(ren, 5, 5, 20, 255);
            SDL_RenderClear(ren);

            player.draw(ren);
            for (const auto& b : bullets) b->draw(ren);
            for (const auto& en : enemies) en->draw(ren);
            draw_hud();

            SDL_RenderPresent(ren);
        }
    }
};

int main(int, char**) {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
