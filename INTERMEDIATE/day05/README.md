# INTERMEDIATE Day 05 — Move Semantics

## Objectives
- Understand lvalues and rvalues
- Write move constructor and move assignment operator
- Use `std::move` correctly
- Benchmark copy vs move performance
- Know the Rule of Five and when it applies

## Key Concepts
- **rvalue reference** `T&&` — binds to temporaries and `std::move()` results
- **Move constructor** `T(T&&)` — steals resources from the source, leaves it valid-but-empty
- **Move assignment** `T& operator=(T&&)` — same but also frees existing resources first
- **`std::move(x)`** — casts x to rvalue reference, enabling move; x is "moved-from" after
- **noexcept** on move operations: required for `std::vector` to use moves during reallocation
- **NRVO** (Named Return Value Optimization): compiler elides the move/copy on return entirely

## Rule of Five
If you define any of: destructor, copy ctor, copy assign, move ctor, move assign — define all five.
But if your class only uses RAII types (string, vector, unique_ptr), you don't need to write any.

## How to Build & Run
```bat
build.bat
lesson.exe
```

## Time Estimate
~2 hours
