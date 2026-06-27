# Day 05 — Functions

## Objectives
- Write reusable functions with correct prototypes
- Use overloading for functions that do the same thing to different types
- Understand the difference between pass-by-value, pass-by-reference, and pass-by-pointer
- Write recursive functions

## Concepts Covered
- Function declarations (prototypes) vs definitions
- Return types and `void`
- Pass-by-value (copy), `&` reference, `const&`, `*` pointer
- Function overloading — same name, different parameter types
- Default arguments — rightmost parameters only
- Recursion and the call stack

## Key Rules
- Prefer `const T&` for read-only large parameters (avoids copy)
- Prefer `T&` for output/in-out parameters (over pointers where possible)
- Default arguments must be rightmost in the parameter list

## How to Build & Run
```bat
build.bat
lesson.exe
mini_project.exe
```

## Time Estimate
~1.5 hours
