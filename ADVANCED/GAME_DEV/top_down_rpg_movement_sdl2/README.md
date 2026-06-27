# Top-Down RPG Movement (SDL2, C++)

Tile-based world navigation with camera, wall collision, and minimap.

## C++ Concepts
- **2D array tilemap**: `const int MAP[MAP_H][MAP_W]` — static layout
- **AABB collision with push-out**: axes separated (move X then Y) to slide along walls
- **Camera clamping**: `cam_x/y` computed from player world pos, clamped to map edges
- **Culled tile rendering**: only draws tiles visible in the current viewport
- **RAII SDL wrappers**: SDLContext, Window, Renderer

## Controls
| Key | Action |
|-----|--------|
| Arrow keys / WASD | Move (8-directional, diagonal normalized) |
| ESC | Quit |

## Tile Types
| Color | Tile |
|-------|------|
| Dark grey | Floor (walkable) |
| Brown | Wall (solid) |
| Blue | Water (solid — no swimming) |
| Gold | Door (walkable) |

## Build & Run
```bat
build.bat
rpg_movement.exe
```

## Key Algorithm: Slide Collision
Movement is split into X and Y axes independently, so the player
slides along walls instead of stopping dead:
```cpp
move_with_collision(dx, 0);  // try horizontal
move_with_collision(0, dy);  // try vertical
```
Each call checks the four corners of the player bounding box against
the tilemap and cancels movement on that axis if any corner hits a solid tile.
