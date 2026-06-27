# INTERMEDIATE Day 08 — Design Patterns I: Singleton, Factory, Builder

## Objectives
- Implement and understand Singleton, Factory, and Builder patterns in modern C++
- Use lambdas and `std::function` for flexible factories
- Apply method chaining in the Builder pattern
- Know when (and when NOT) to use Singleton

## Patterns
### Singleton
- Only one instance of the class ever exists
- Lazily initialized with `static Logger inst;` (thread-safe in C++11)
- Drawback: hard to test; avoid over-using

### Factory (Method / Abstract)
- Creates objects without specifying the exact class at compile time
- Registration map of creators (`map<string, function<unique_ptr<T>()>>`)
- New types can be added without changing factory code (open/closed principle)

### Builder
- Constructs complex objects step-by-step
- Method chaining returns `*this` reference
- `.build()` produces the final object
- Great for objects with many optional parameters

## How to Build & Run
```bat
build.bat
lesson.exe
```

## Time Estimate
~2 hours
