# INTERMEDIATE Day 07 — Unit Testing with doctest

## Objectives
- Write real unit tests for C++ code
- Use doctest — a header-only framework that requires no installation
- Structure testable code (separate library code from test code)
- Understand what to test and how to write good test cases

## doctest Setup
1. Download `doctest.h` (single file) from: https://github.com/doctest/doctest/releases
2. Copy it next to `lesson.cpp`
3. Run `build.bat` — doctest will be detected automatically

Without `doctest.h`, a manual fallback test runner is used (still verifies correctness).

## How to Build & Run
```bat
build.bat
tests.exe
```

## What Good Tests Cover
- Happy path: normal inputs
- Edge cases: empty, single element, boundary values
- Error cases: what should throw, what should return default
- Regression: known buggy inputs that were fixed

## Key doctest API
```cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("description") {
    CHECK(result == expected);
    CHECK_FALSE(bad_condition);
    CHECK_THROWS_AS(fn(), std::exception_type);
    REQUIRE(critical);  // stops test on failure
    CHECK(val == doctest::Approx(3.14));  // floating-point comparison
}
```

## Time Estimate
~1.5 hours
