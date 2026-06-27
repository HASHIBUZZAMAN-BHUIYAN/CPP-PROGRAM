# Day 10 — OOP II: Inheritance, Virtual Functions, Polymorphism

## Objectives
- Build class hierarchies with public inheritance
- Use virtual functions for runtime polymorphism
- Create and use abstract base classes (pure virtual)
- Downcast safely with `dynamic_cast`

## Concepts Covered
- `class Derived : public Base` — inheritance
- `virtual` functions and the vtable (runtime dispatch)
- `override` keyword (C++11) — compiler-verified override
- Pure virtual functions `= 0` — makes class abstract
- Virtual destructor — required in any base class with virtual methods
- `dynamic_cast<Derived*>(base_ptr)` — safe downcast (returns nullptr on failure)
- Slicing problem — why you need pointers/references for polymorphism

## Key Rules
- **Always** declare destructors `virtual` in base classes
- **Always** use `override` on overriding functions
- A class with any pure virtual is abstract — can't instantiate it directly

## How to Build & Run
```bat
build.bat
lesson.exe
mini_project.exe
```

## Time Estimate
~2 hours
