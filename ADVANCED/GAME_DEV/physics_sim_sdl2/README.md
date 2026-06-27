# Physics Simulation — Bouncing Balls (SDL2, C++)

Interactive bouncing ball physics with gravity, wall bounce, and ball-ball collision.

## C++ Concepts
- **Vec2 struct**: operator overloading for 2D vector math (`+`, `-`, `*`, `dot`, `norm`)
- **Verlet-style integration**: `pos += vel * dt` with gravity added each frame
- **Elastic collision response**: impulse-based velocity exchange along collision normal
- **Object pool pattern**: fixed-capacity `std::vector<Ball>`, oldest removed on overflow
- **RAII SDL wrappers**: no explicit cleanup calls needed

## Controls
| Input | Action |
|-------|--------|
| Left click | Spawn a ball at mouse position |
| Right click | Clear all balls |
| ESC | Quit |

## Build & Run
```bat
build.bat
physics_sim.exe
```

## Physics Model
```
Each frame:
  vel.y += GRAVITY * dt          (9.8 m/s^2, scaled to pixels)
  pos   += vel * dt              (Euler integration)
  RESTITUTION = 0.75             (75% energy retained on bounce)

Ball-ball collision:
  1. Check overlap: dist < r_a + r_b
  2. Push apart by (overlap / 2) along collision normal
  3. Exchange velocity components along normal (elastic impulse)
```

Ball-ball is O(n²) which is fine for ≤ 80 balls. For hundreds of balls,
you'd use a spatial hash or broad-phase BVH.
