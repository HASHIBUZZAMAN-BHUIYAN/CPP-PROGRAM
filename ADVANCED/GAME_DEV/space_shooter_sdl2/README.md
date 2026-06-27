# Space Shooter (SDL2, C++)

2D space shooter demonstrating OOP game architecture with SDL2.

## C++ Concepts
- **Polymorphism**: Entity base class → Player, Enemy, Bullet derived
- **RAII**: SDLContext, Window, Renderer wrappers — no leaks on exception
- **Smart pointers**: `unique_ptr<Entity>` for bullet/enemy containers
- **STL algorithms**: `remove_if` + `erase` for dead entity cleanup
- **Delta-time game loop**: frame-rate independent movement

## Controls
| Key | Action |
|-----|--------|
| Arrow keys / WASD | Move |
| SPACE | Shoot |
| ESC | Quit |

## Build & Run
```bat
build.bat
space_shooter.exe
```

## Architecture
```
Entity (abstract)
├── Player  — keyboard input, fire rate limiting
├── Enemy   — sinusoidal wobble, bounces off walls
└── Bullet  — straight-line trajectory, auto-deactivate off-screen

Game
├── std::vector<unique_ptr<Bullet>>
├── std::vector<unique_ptr<Enemy>>
├── AABB collision detection
└── HUD rendered with colored rects (no font required)
```
