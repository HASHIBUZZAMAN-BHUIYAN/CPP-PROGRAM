# INTERMEDIATE Day 04 — Smart Pointers

## Objectives
- Replace all raw pointer ownership with smart pointers
- Understand `unique_ptr`, `shared_ptr`, and `weak_ptr` ownership semantics
- Break reference cycles with `weak_ptr`
- Know when to use each

## Summary
| Smart Pointer | Ownership | Copy | Move |
|---------------|-----------|------|------|
| `unique_ptr` | Sole (exclusive) | ❌ | ✅ |
| `shared_ptr` | Shared (ref-counted) | ✅ | ✅ |
| `weak_ptr` | None (observer) | ✅ | ✅ |

## Rules
- Default to `unique_ptr` — zero overhead, clear ownership
- Use `shared_ptr` only when multiple owners are genuinely needed
- Use `weak_ptr` to break cycles or for non-owning observers
- Always use `make_unique` / `make_shared` — never `new` directly
- Raw pointers are fine as **non-owning observers** (like references)

## How to Build & Run
```bat
build.bat
lesson.exe
```

## Time Estimate
~2 hours
