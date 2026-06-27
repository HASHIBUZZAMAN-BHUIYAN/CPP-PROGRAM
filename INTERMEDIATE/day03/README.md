# INTERMEDIATE Day 03 — STL Algorithms & Lambdas

## Objectives
- Replace hand-written loops with STL algorithms
- Write lambdas for inline predicates and transformations
- Capture variables by value and reference correctly
- Use `std::function` to store and pass callable objects

## Key Algorithms
| Algorithm | What it does |
|-----------|-------------|
| `std::transform` | Apply function to each element |
| `std::accumulate` | Fold/reduce a range |
| `std::reduce` | Like accumulate, parallelizable (C++17) |
| `std::stable_sort` | Sort preserving relative order of equals |
| `std::generate` | Fill range with generated values |
| `std::adjacent_difference` | Differences between consecutive elements |
| `std::for_each` | Apply function for side effects |

## Lambda Syntax
```cpp
[capture](params) -> return_type { body }
[]      // capture nothing
[=]     // capture all by value
[&]     // capture all by reference
[x, &y] // capture x by value, y by reference
[n = 0]() mutable { return ++n; }  // initializer capture (C++14)
```

## How to Build & Run
```bat
build.bat
lesson.exe
```

## Time Estimate
~2 hours
