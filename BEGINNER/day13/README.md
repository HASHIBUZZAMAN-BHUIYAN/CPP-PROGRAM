# Day 13 — Intro to the STL

## Objectives
- Use the most important STL containers correctly
- Apply STL algorithms with lambdas instead of hand-written loops
- Understand iterators as the bridge between containers and algorithms
- Choose the right container for each use case

## Concepts Covered
| Container | Key property |
|-----------|-------------|
| `std::vector` | Resizable array, O(1) access |
| `std::map<K,V>` | Sorted key-value, O(log n) ops |
| `std::set<T>` | Sorted unique values |
| `std::unordered_map<K,V>` | Hash map, O(1) avg |
| `std::stack` | LIFO adapter |
| `std::queue` | FIFO adapter |
| `std::priority_queue` | Heap, max on top |

Key algorithms: `std::sort`, `std::find_if`, `std::count_if`, `std::transform`,
`std::accumulate`, `std::partition`, `std::all_of/any_of/none_of`, `std::set_union`, `std::iota`

## How to Build & Run
```bat
build.bat
lesson.exe
mini_project.exe
```

## Time Estimate
~2 hours
