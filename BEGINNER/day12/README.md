# Day 12 — Exception Handling

## Objectives
- Throw and catch exceptions for recoverable error conditions
- Use the standard exception hierarchy correctly
- Write custom exception classes
- Apply RAII to ensure resources are released even when exceptions occur

## Concepts Covered
- `throw expression` — raises an exception
- `try { } catch (const ExcType& e) { }` — handles it
- `catch (...)` — catch-all (use sparingly)
- Standard exceptions: `std::runtime_error`, `std::logic_error`, `std::invalid_argument`, `std::out_of_range`, `std::domain_error`
- Custom exceptions: inherit from `std::exception` or its subclasses
- Catch order: most-derived first, least-derived last
- Re-throwing with bare `throw;`
- `noexcept` — marks a function that never throws (helps optimizer + move semantics)
- RAII + exceptions: destructors run on stack unwind

## How to Build & Run
```bat
build.bat
lesson.exe
mini_project.exe
```

## Time Estimate
~1.5 hours
