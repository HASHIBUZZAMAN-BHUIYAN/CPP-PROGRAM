# Day 08 — Pointers & References

## Objectives
- Understand memory addresses and how pointers store them
- Use `&` references and `*` pointers correctly
- Know when to use each, and why modern C++ prefers references
- Understand pointer arithmetic and its relationship to arrays

## Concepts Covered
- `&` address-of operator vs `&` reference declaration
- `*` dereference operator
- Pointer arithmetic (`p + 1`, `*(p + i)`)
- Array/pointer relationship (array name decays to pointer)
- `nullptr` (C++11) — always use instead of NULL or 0
- `const int*` vs `int* const` vs `const int* const`
- References: must initialize, can't be null, can't reseat
- Pointers: can be null, can be reassigned, can do arithmetic

## Key Rule
Prefer references. Use pointers when you need null-ability, reassignment, or arithmetic.
In Day 04 of INTERMEDIATE you'll replace raw pointers entirely with smart pointers.

## How to Build & Run
```bat
build.bat
lesson.exe
mini_project.exe
```

## Time Estimate
~1.5 hours
