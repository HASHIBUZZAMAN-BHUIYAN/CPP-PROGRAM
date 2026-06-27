# INTERMEDIATE Day 10 — Capstone: Task Manager (CMake + Smart Ptrs + Design Patterns + Tests)

## Objectives
- Apply all INTERMEDIATE concepts in one structured CMake project
- Use smart pointers throughout (no raw `new`/`delete`)
- Apply at least 2 design patterns
- Write a test suite using doctest or the manual fallback

## What's Built
A task management library with:
- **Factory** to create different task types (Bug, Feature, Chore)
- **Observer** to notify when tasks change state
- **Builder** to construct complex tasks step-by-step
- **Smart pointers** everywhere — no raw memory management
- **Templates** for the generic task collection
- **Tests** covering all public APIs

## Project Structure
```
day10/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── task.cpp
│   └── task_manager.cpp
├── include/
│   ├── task.h
│   └── task_manager.h
└── tests/
    └── test_tasks.cpp
```

## How to Build & Run
```bat
cmake -B build -G "MinGW Makefiles"
cmake --build build
build\task_app.exe
build\task_tests.exe
```

## Time Estimate
~3-4 hours (full project)
