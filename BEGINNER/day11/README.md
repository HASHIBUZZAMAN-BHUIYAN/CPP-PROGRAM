# Day 11 — Operator Overloading & Rule of Three/Five

## Objectives
- Overload operators to make user-defined types feel natural
- Understand the Rule of Three (destructor + copy ctor + copy assignment)
- Know why the Rule of Three is needed when you own raw resources
- Preview the Rule of Five (adds move constructor + move assignment, Day 05 INTERMEDIATE)

## Concepts Covered
- Overloading `+  -  *  /  +=  -=` (arithmetic)
- Overloading `==  !=  <  >` (comparison — makes container sorting work)
- Overloading `<<` as a `friend` non-member for stream output
- Overloading `[]` subscript and `()` call operator
- Rule of Three: if you write any of destructor/copy-ctor/copy-assignment, write all three
- Self-assignment guard in copy assignment: `if (this == &other) return *this;`

## Key Insight
If your class only uses RAII types (std::string, std::vector, std::unique_ptr etc.)
you don't need to write any of the Rule of Three/Five — the compiler generates correct ones.
Raw pointers and raw arrays break this, which is why we avoid them.

## How to Build & Run
```bat
build.bat
lesson.exe
mini_project.exe
```

## Time Estimate
~2 hours
