# Particle System — Object Pool (SDL2, C++)

High-performance particle system using an object pool to avoid per-frame heap allocation.

## C++ Concepts
- **Object pool pattern**: `std::array<Particle, 2000>` — fixed allocation, zero `new`/`delete` in the main loop
- **Additive blending**: `SDL_BLENDMODE_ADD` makes overlapping particles glow brighter
- **Alpha fade by lifetime**: `alpha = (life / max_life) * 220`
- **Data-oriented design**: Particle is plain data (no vtable), laid out contiguously — cache-friendly iteration
- **Function pointer dispatch**: emitter type determines which `emit_*` function runs

## Controls
| Key | Emitter |
|-----|---------|
| 1 | Fire (continuous) |
| 2 | Explosion (burst on click) |
| 3 | Snow (continuous, from top) |
| 4 | Fountain (continuous) |
| Click | Move emitter (or re-trigger explosion) |
| ESC | Quit |

## Build & Run
```bat
build.bat
particle_system.exe
```

## Why Object Pool?
Without a pool, spawning 100 particles per frame = 100 `new` calls per frame = 6000 allocations/sec.
Each allocation may trigger heap fragmentation and cache misses.

With a pool: zero allocations after startup. The pool is a flat array — iteration
visits contiguous memory = L1 cache stays warm = consistent frame times.

```
Pool state per frame:
  alloc() — O(1) round-robin slot search
  update() — O(N) linear scan, branch-predicted inactive skip
  draw()   — O(N) linear scan, additive blend to renderer
```
