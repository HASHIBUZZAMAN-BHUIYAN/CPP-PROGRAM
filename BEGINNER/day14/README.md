# Day 14 — Capstone: Contact Manager

## Objectives
- Apply everything from Days 1-13 in one complete program
- Design a multi-class system with proper separation of concerns
- Handle all error cases with exceptions
- Persist data with file I/O

## Concepts Applied
- **Classes** (Day 09): `Contact`, `ContactBook`
- **STL** (Day 13): `std::map`, `std::vector`, `std::find`, `std::transform`
- **Exceptions** (Day 12): validation with `std::invalid_argument`, `std::runtime_error`
- **Strings** (Day 07): parsing, `getline`, `substr`, `find`
- **File I/O**: save to CSV, load from CSV
- **Operator overloading** (Day 11): `operator<` for sorting

## How to Build & Run
```bat
build.bat
contact_manager.exe
```

## Features
1. Add / remove / edit contacts
2. Search by name substring
3. List all (sorted alphabetically by key in map)
4. Save to / load from CSV file

## What to Try Next
- Add categories or tags to contacts
- Sort by different fields
- Add a duplicate phone number check
- Export to JSON format (preview of TRENDING day 03)

## Time Estimate
~2-3 hours (this is a full project)
