// ADVANCED GAME_DEV: Top-Down RPG Movement (SDL2, C++)
//
// WHAT THIS DEMONSTRATES:
// - Tile-based map representation (2D array)
// - Wall collision with AABB
// - Camera following the player (viewport offset)
// - Smooth sub-tile movement with collision push-out
//
// CONTROLS: Arrow keys / WASD to move, ESC to quit

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

// ============================================================
// SDL2 RAII wrappers (same pattern as space_shooter)
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
const int SCREEN_W   = 800, SCREEN_H   = 600;
const int TILE_SIZE  = 32;
const float PLAYER_SPEED = 180.0f;

// ============================================================
// Tilemap
// ============================================================
// 0 = floor, 1 = wall, 2 = water, 3 = door
const int MAP_W = 25, MAP_H = 20;

const int MAP[MAP_H][MAP_W] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,1,0,0,1,1,1,0,0,1,0,0,1,1,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,1,0,0,0,0,1,0,0,3,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,1,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1},
    {1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,2,2,2,2,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

bool is_solid(int tile) { return tile == 1; }

SDL_Color tile_color(int t) {
    switch (t) {
        case 0: return {60, 60, 70, 255};    // floor
        case 1: return {100, 80, 60, 255};   // wall
        case 2: return {30, 80, 160, 255};   // water
        case 3: return {160, 120, 40, 255};  // door
        default: return {0, 0, 0, 255};
    }
}

// ============================================================
// Player
// ============================================================
class Player {
public:
    float x, y;   // world position (center)
    float w, h;

    Player() : x(2.5f * TILE_SIZE), y(2.5f * TILE_SIZE), w(22), h(22) {}

    void update(float dt) {
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        float dx = 0, dy = 0;
        if (keys[SDL_SCANCODE_LEFT]  || keys[SDL_SCANCODE_A]) dx -= PLAYER_SPEED * dt;
        if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) dx += PLAYER_SPEED * dt;
        if (keys[SDL_SCANCODE_UP]    || keys[SDL_SCANCODE_W]) dy -= PLAYER_SPEED * dt;
        if (keys[SDL_SCANCODE_DOWN]  || keys[SDL_SCANCODE_S]) dy += PLAYER_SPEED * dt;

        // Normalize diagonal
        if (dx != 0 && dy != 0) {
            dx *= 0.7071f;
            dy *= 0.7071f;
        }

        move_with_collision(dx, 0);
        move_with_collision(0, dy);
    }

    void draw(SDL_Renderer* ren, float cam_x, float cam_y) const {
        SDL_SetRenderDrawColor(ren, 0, 220, 100, 255);
        SDL_FRect r{x - w/2 - cam_x, y - h/2 - cam_y, w, h};
        SDL_RenderFillRectF(ren, &r);
        // Arrow indicator (direction not tracked here — just a dot)
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_FRect dot{x - 3 - cam_x, y - 3 - cam_y, 6, 6};
        SDL_RenderFillRectF(ren, &dot);
    }

private:
    void move_with_collision(float dx, float dy) {
        float nx = x + dx, ny = y + dy;

        // Check the four corners of the player bounding box
        float half_w = w/2 - 1, half_h = h/2 - 1;
        float corners_x[] = {nx - half_w, nx + half_w, nx - half_w, nx + half_w};
        float corners_y[] = {ny - half_h, ny - half_h, ny + half_h, ny + half_h};

        for (int i = 0; i < 4; ++i) {
            int tx = static_cast<int>(corners_x[i]) / TILE_SIZE;
            int ty = static_cast<int>(corners_y[i]) / TILE_SIZE;
            if (tx < 0 || ty < 0 || tx >= MAP_W || ty >= MAP_H) return;
            if (is_solid(MAP[ty][tx])) return;
        }

        x = nx;
        y = ny;
    }
};

// ============================================================
// Game
// ============================================================
class Game {
    SDLContext sdl;
    Window     window;
    Renderer   renderer;
    Player     player;
    bool       running;

    void draw_map(float cam_x, float cam_y) {
        SDL_Renderer* ren = renderer.ptr;
        int start_tx = static_cast<int>(cam_x) / TILE_SIZE;
        int start_ty = static_cast<int>(cam_y) / TILE_SIZE;
        int end_tx = start_tx + SCREEN_W / TILE_SIZE + 2;
        int end_ty = start_ty + SCREEN_H / TILE_SIZE + 2;

        for (int ty = std::max(0, start_ty); ty < std::min(MAP_H, end_ty); ++ty) {
            for (int tx = std::max(0, start_tx); tx < std::min(MAP_W, end_tx); ++tx) {
                SDL_Color c = tile_color(MAP[ty][tx]);
                SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
                SDL_Rect r{
                    tx * TILE_SIZE - static_cast<int>(cam_x),
                    ty * TILE_SIZE - static_cast<int>(cam_y),
                    TILE_SIZE - 1, TILE_SIZE - 1
                };
                SDL_RenderFillRect(ren, &r);

                // Draw wall edge highlight
                if (MAP[ty][tx] == 1) {
                    SDL_SetRenderDrawColor(ren, 140, 110, 85, 255);
                    SDL_Rect top{r.x, r.y, TILE_SIZE - 1, 3};
                    SDL_RenderFillRect(ren, &top);
                }
            }
        }
    }

    void draw_minimap() {
        const int MINI_SCALE = 4;
        const int MINI_X = SCREEN_W - MAP_W * MINI_SCALE - 10;
        const int MINI_Y = 10;
        SDL_Renderer* ren = renderer.ptr;

        // Background
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 180);
        SDL_Rect bg{MINI_X - 2, MINI_Y - 2, MAP_W * MINI_SCALE + 4, MAP_H * MINI_SCALE + 4};
        SDL_RenderFillRect(ren, &bg);

        for (int ty = 0; ty < MAP_H; ++ty) {
            for (int tx = 0; tx < MAP_W; ++tx) {
                SDL_Color c = tile_color(MAP[ty][tx]);
                SDL_SetRenderDrawColor(ren, c.r/2, c.g/2, c.b/2, 255);
                SDL_Rect r{MINI_X + tx * MINI_SCALE, MINI_Y + ty * MINI_SCALE, MINI_SCALE, MINI_SCALE};
                SDL_RenderFillRect(ren, &r);
            }
        }
        // Player dot on minimap
        SDL_SetRenderDrawColor(ren, 0, 255, 100, 255);
        int px = MINI_X + static_cast<int>(player.x / TILE_SIZE * MINI_SCALE);
        int py = MINI_Y + static_cast<int>(player.y / TILE_SIZE * MINI_SCALE);
        SDL_Rect pdot{px - 2, py - 2, 4, 4};
        SDL_RenderFillRect(ren, &pdot);
    }

public:
    Game() : window("C++ Top-Down RPG Movement (SDL2)", SCREEN_W, SCREEN_H),
             renderer(window.ptr), running(true) {}

    void run() {
        Uint32 last_ticks = SDL_GetTicks();

        while (running) {
            Uint32 now = SDL_GetTicks();
            float dt = std::min((now - last_ticks) / 1000.0f, 0.05f);
            last_ticks = now;

            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) running = false;
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
            }

            player.update(dt);

            // Camera: center player on screen, clamp to map bounds
            float cam_x = player.x - SCREEN_W / 2.0f;
            float cam_y = player.y - SCREEN_H / 2.0f;
            float max_cam_x = MAP_W * TILE_SIZE - SCREEN_W;
            float max_cam_y = MAP_H * TILE_SIZE - SCREEN_H;
            if (cam_x < 0) cam_x = 0;
            if (cam_y < 0) cam_y = 0;
            if (cam_x > max_cam_x) cam_x = max_cam_x;
            if (cam_y > max_cam_y) cam_y = max_cam_y;

            // Render
            SDL_Renderer* ren = renderer.ptr;
            SDL_SetRenderDrawColor(ren, 20, 20, 30, 255);
            SDL_RenderClear(ren);

            draw_map(cam_x, cam_y);
            player.draw(ren, cam_x, cam_y);
            draw_minimap();

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
