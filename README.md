# CPP-PROGRAM — Complete C++ Curriculum

A structured C++ learning path from fundamentals to advanced systems programming,
built and verified on Windows (Ryzen 7 5000-series, 8 GB RAM, no GPU).

## Toolchain

| Tool | Version | Notes |
|------|---------|-------|
| g++ (MinGW-W64 POSIX UCRT) | 16.1.0 | C++17 and C++20 fully supported |
| CMake | 4.3.2 | MinGW Makefiles generator |
| SDL2 | 2.32.8 | Shared from `C-PROGRAM/SDL2/SDL2-2.32.8/` |

**Quick compile (single file):**
```bat
g++ -std=c++17 -Wall -O2 -o program.exe program.cpp
```

**Quick compile (SDL2):**
```bat
set SDL2=C:\Users\zen\Documents\GitHub\C-PROGRAM\SDL2\SDL2-2.32.8\x86_64-w64-mingw32
g++ -std=c++17 -Wall -O2 -o game.exe game.cpp -I"%SDL2%\include" -L"%SDL2%\lib" -lmingw32 -lSDL2main -lSDL2 -mwindows
copy "%SDL2%\bin\SDL2.dll" .
```

**Quick build (CMake project):**
```bat
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

---

## BEGINNER Track — 14 Days

Each day: `lesson.cpp` · `exercises.cpp` · `solutions.cpp` · `mini_project.cpp` · `build.bat` · `README.md`

| Day | Topic | Mini-Project |
|-----|-------|-------------|
| 01 | Basics — `#include`, `cout/cin`, variables, types | Personal Info Card |
| 02 | Operators, type casting, `const`, `<cmath>` | Unit Converter |
| 03 | Conditionals — `if/else`, `switch`, ternary | Number Guessing Game |
| 04 | Loops — `for`, `while`, `do-while`, range-based | Multiplication Table Generator |
| 05 | Functions — overloading, default args, refs vs ptrs | Math Library |
| 06 | Arrays — C-array, `std::array`, `std::vector` | Grade Book |
| 07 | Strings — `std::string` methods, `istringstream` | Text Statistics Analyzer |
| 08 | Pointers & references — pointer arithmetic | Dynamic Array (manual) |
| 09 | OOP I — classes, constructors, destructors, `static` | Library Catalog |
| 10 | OOP II — inheritance, `virtual`, polymorphism, `dynamic_cast` | Shape Drawing |
| 11 | Operator overloading — Rule of Three | Calculator with History |
| 12 | Exception handling — `try/catch/throw`, RAII | CSV Reader |
| 13 | STL intro — `map`, `set`, `unordered_map`, algorithms | Task Manager |
| 14 | **Capstone** | Contact Manager (full CRUD) |

**Run a day:**
```bat
cd BEGINNER\day01
build.bat
```

---

## INTERMEDIATE Track — 10 Days

| Day | Topic | Key Concepts |
|-----|-------|-------------|
| 01 | Templates | Function/class templates, specialization, `Stack<T>`, `Pair<T,U>` |
| 02 | STL Containers Deep Dive | `list`, `set`, `multiset`, `unordered_map`, `deque`, benchmarks |
| 03 | STL Algorithms & Lambdas | `transform`, `accumulate`, `reduce`, `generate`, `std::function` |
| 04 | Smart Pointers | `unique_ptr`, `shared_ptr`, `weak_ptr`, breaking cycles |
| 05 | Move Semantics | Buffer with Rule of Five, copy vs move vs emplace benchmark |
| 06 | Intro to CMake | `CMakeLists.txt`, header-only `mathlib` namespace |
| 07 | Unit Testing | doctest (`__has_include` auto-detect), fallback test runner |
| 08 | Design Patterns I | Logger Singleton, `AnimalFactory`, `PizzaBuilder` |
| 09 | Design Patterns II | Observer `EventSystem`, Strategy, RAII `FileGuard`/`ScopedTimer` |
| 10 | **Capstone (CMake)** | Task management app — static library + executable + tests |

**Run capstone:**
```bat
cd INTERMEDIATE\day10
cmake -B build -G "MinGW Makefiles"
cmake --build build
build\task_app.exe
build\task_tests.exe
```

---

## ADVANCED Track

### ADVANCED\SYSTEMS_PERFORMANCE

```bat
cd ADVANCED\SYSTEMS_PERFORMANCE
build.bat
```

| Program | Concepts |
|---------|---------|
| `custom_memory_pool.cpp` | Fixed-block pool, intrusive free list, benchmark vs `new/delete` |
| `raii_resource_management.cpp` | `FileHandle`, `MutexGuard`, `ScopeGuard` — RAII patterns |
| `move_semantics_optimization.cpp` | Rule of Five, `push_back` vs `emplace_back` benchmark |
| `multithreading_modern_cpp.cpp` | `std::thread`, `std::atomic`, `std::mutex`, `condition_variable`, `async/future` |
| `profiling_and_benchmarking.cpp` | `std::chrono` harness, sort/lookup/memory-access benchmarks |

---

### ADVANCED\GAME_DEV (SDL2)

All games copy `SDL2.dll` automatically on first `build.bat`.

```bat
cd ADVANCED\GAME_DEV\space_shooter_sdl2
build.bat && space_shooter.exe
```

| Project | Concepts |
|---------|---------|
| `space_shooter_sdl2\` | Entity hierarchy, AABB collision, `unique_ptr` pool, RAII SDL wrappers |
| `top_down_rpg_movement_sdl2\` | Tile-based map, slide collision, camera clamping, minimap |
| `physics_sim_sdl2\` | Vec2 ops, Euler integration, elastic ball-ball collision |
| `particle_system_sdl2\` | Object pool (`std::array<Particle,2000>`), additive blend, 4 emitter modes |

---

### ADVANCED\ROBOTICS_CV

```bat
cd ADVANCED\ROBOTICS_CV
build.bat
```

| Program | Concepts |
|---------|---------|
| `eigen_linear_algebra_robotics.cpp` | 4×4 homogeneous transforms, rotation matrices, 3-DOF forward kinematics, DH params |
| `pid_controller_cpp.cpp` | PID class, anti-windup, derivative-on-measurement, disturbance rejection, multi-axis |
| `opencv_basic_vision_cpp.cpp` | Grayscale, 2D convolution, Gaussian blur, Sobel edges, connected components |
| `ros2_style_node_pattern_cpp.cpp` | Simulated rclcpp Node/Publisher/Subscriber/Timer/Executor (no ROS install needed) |

> No external dependencies required. Eigen and OpenCV are explained; see each file's header for install instructions.

---

### ADVANCED\TRENDING

```bat
cd ADVANCED\TRENDING
build.bat
```

| Program | Concepts |
|---------|---------|
| `modern_cpp20_features_demo.cpp` | Concepts, Ranges, `std::span`, spaceship `<=>`, designated init, `consteval`, `std::format` |
| `network_socket_basics_cpp.cpp` | Winsock2 TCP echo server+client, RAII socket wrappers, `std::thread` |
| `json_parsing_modern_cpp.cpp` | Recursive descent JSON parser, `std::variant`, `std::visit`, nlohmann/json API |
| `cmake_multi_module_project\` | Static library (`mathutils`), headers/src/tests split, CMake targets |

**CMake multi-module project:**
```bat
cd ADVANCED\TRENDING\cmake_multi_module_project
cmake -B build -G "MinGW Makefiles"
cmake --build build
build\math_app.exe       -- demo application
build\math_tests.exe     -- 30 tests, all pass
```

---

## Quick-Start: Verify Everything Builds

```bat
cd BEGINNER\day01 && build.bat && cd ..\..
cd INTERMEDIATE\day01 && build.bat && cd ..\..
cd ADVANCED\SYSTEMS_PERFORMANCE && build.bat && cd ..\..
cd ADVANCED\GAME_DEV\space_shooter_sdl2 && build.bat && cd ..\..\..
cd ADVANCED\ROBOTICS_CV && build.bat && cd ..\..
cd ADVANCED\TRENDING && build.bat && cd ..\..
```

---

## Directory Structure

```
CPP-PROGRAM\
├── README.md
├── CPP_SETUP.md
│
├── BEGINNER\
│   └── day01\ ... day14\
│       ├── lesson.cpp
│       ├── exercises.cpp
│       ├── solutions.cpp
│       ├── mini_project.cpp
│       ├── build.bat
│       └── README.md
│
├── INTERMEDIATE\
│   └── day01\ ... day10\
│
└── ADVANCED\
    ├── SYSTEMS_PERFORMANCE\     5 programs + build.bat
    ├── GAME_DEV\                4 SDL2 games, each with build.bat + README.md
    ├── ROBOTICS_CV\             4 programs + build.bat
    └── TRENDING\                3 standalone + 1 CMake project + build.bat
```

---

## Learning Path

```
BEGINNER  day01-07  →  syntax, control flow, functions, strings
BEGINNER  day08-14  →  OOP, memory, STL, exceptions, capstone
INTERMEDIATE 01-05  →  templates, STL deep dive, smart ptrs, move semantics
INTERMEDIATE 06-10  →  CMake, testing, design patterns, capstone
ADVANCED/SYSTEMS    →  production-grade memory and concurrency patterns
ADVANCED/GAME_DEV   →  apply OOP + SDL2 for real-time interactive programs
ADVANCED/ROBOTICS   →  domain application of linear algebra and control theory
ADVANCED/TRENDING   →  C++20, networking, JSON, multi-module CMake
```
