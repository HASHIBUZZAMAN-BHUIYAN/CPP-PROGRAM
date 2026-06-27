// INTERMEDIATE Day 06 — Intro to CMake
// This file is the main source; see CMakeLists.txt for the build system setup.

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "mathlib.h"   // our own header — included via CMake's target_include_directories

int main() {
    std::cout << "=== INTERMEDIATE Day 06: CMake Project ===\n\n";

    // Use functions from our mathlib
    std::cout << "MathLib demo:\n";
    std::cout << "  add(3, 4)      = " << mathlib::add(3, 4)      << "\n";
    std::cout << "  multiply(6, 7) = " << mathlib::multiply(6, 7) << "\n";
    std::cout << "  factorial(10)  = " << mathlib::factorial(10)  << "\n";
    std::cout << "  is_prime(17)   = " << std::boolalpha << mathlib::is_prime(17) << "\n";
    std::cout << "  circle_area(5) = " << mathlib::circle_area(5.0) << "\n";

    std::cout << "\nThis program was built with CMake.\n";
    std::cout << "See CMakeLists.txt for the build configuration.\n";
    std::cout << "Build with:\n";
    std::cout << "  cmake -B build -G \"MinGW Makefiles\"\n";
    std::cout << "  cmake --build build\n";
    std::cout << "  build\\day06_app.exe\n";

    return 0;
}
