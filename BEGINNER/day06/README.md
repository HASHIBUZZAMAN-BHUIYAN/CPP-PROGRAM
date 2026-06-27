# Day 06 — Arrays, std::array, std::vector

## Objectives
- Understand C-style arrays and their limitations
- Use `std::array` for fixed-size collections
- Use `std::vector` as the default resizable container
- Apply common STL algorithms to arrays and vectors

## Concepts Covered
- Raw arrays: declaration, indexing, `sizeof` trick, limitations
- `std::array<T, N>`: fixed size, knows its own size, `.at()` bounds checking
- `std::vector<T>`: dynamic, `.push_back()`, `.pop_back()`, `.insert()`, `.erase()`
- `.size()`, `.capacity()`, `.front()`, `.back()`
- 2D vectors: `vector<vector<T>>`
- `std::sort`, `std::reverse`, `std::find`, `std::unique`, `std::rotate`

## When to Use What
| Container | Use when |
|-----------|---------|
| Raw array | Almost never in modern C++ |
| `std::array` | Size known at compile time, stack allocation preferred |
| `std::vector` | Default choice for any resizable collection |

## How to Build & Run
```bat
build.bat
lesson.exe
mini_project.exe
```

## Time Estimate
~1.5 hours
