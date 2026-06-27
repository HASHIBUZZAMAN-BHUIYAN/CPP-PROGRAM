# INTERMEDIATE Day 09 — Design Patterns II: Observer, Strategy, RAII

## Objectives
- Implement the Observer pattern for event-driven programming
- Use the Strategy pattern to make algorithms interchangeable
- Understand RAII as C++'s most important resource management pattern
- Apply `std::function` for flexible, non-polymorphic callbacks

## Patterns

### Observer
- Subject notifies all registered observers when state changes
- Functional style: `std::function<void(data)>` handlers (no class hierarchy needed)
- Classic OOP style: `IObserver` interface + pointer registration
- Used in: GUI events, game engines, message buses, MVC

### Strategy
- Encapsulates an algorithm behind an interface
- Can swap implementations at runtime
- Modern C++ alternative: just use a lambda / `std::function` directly
- Used in: sorting, payment processing, compression, serialization

### RAII
- The single most important C++ pattern
- Resource acquired in constructor, released in destructor
- Works for: file handles, mutexes, connections, memory, timers
- Exception-safe: destructor runs even if an exception is thrown
- **Key insight**: `unique_ptr`, `shared_ptr`, `lock_guard` are all RAII

## How to Build & Run
```bat
build.bat
lesson.exe
```

## Time Estimate
~2 hours
