# INTERMEDIATE Day 02 — STL Containers Deep Dive

## Objectives
- Choose the right container for each use case
- Understand the performance characteristics of each
- Use `std::list` for efficient mid-collection insertions
- Use `std::set`/`std::multiset` for sorted unique/multi collections
- Benchmark `std::map` vs `std::unordered_map`

## Container Performance Summary
| Container | Access | Insert | Delete | Ordered |
|-----------|--------|--------|--------|---------|
| `vector` | O(1) | O(1) back, O(n) mid | O(n) | No |
| `list` | O(n) | O(1) given iter | O(1) given iter | No |
| `deque` | O(1) | O(1) front/back | O(1) front/back | No |
| `set` | O(log n) | O(log n) | O(log n) | Yes |
| `map` | O(log n) | O(log n) | O(log n) | Yes |
| `unordered_map` | O(1) avg | O(1) avg | O(1) avg | No |

## How to Build & Run
```bat
build.bat
lesson.exe
```

## Time Estimate
~1.5 hours
