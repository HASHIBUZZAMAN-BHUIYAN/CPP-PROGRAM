// Day 01 — Exercise Solutions

#include <iostream>
#include <string>

int main() {
    std::cout << "=== Day 01 Solutions ===\n\n";

    // Exercise 1
    std::cout << "Exercise 1:\n";
    std::string myName = "Hashi";
    std::string myLang = "C++";
    std::cout << "My name is " << myName << " and I love " << myLang << ".\n";

    // Exercise 2
    std::cout << "\nExercise 2:\n";
    double x = 7.5, y = 2.0;
    std::cout << "x = " << x << ", y = " << y << "\n";
    std::cout << "Sum:        " << x + y << "\n";
    std::cout << "Difference: " << x - y << "\n";
    std::cout << "Product:    " << x * y << "\n";
    std::cout << "Quotient:   " << x / y << "\n";

    // Exercise 3
    std::cout << "\nExercise 3:\n";
    std::cout << "Enter your birth year: ";
    int birthYear;
    std::cin >> birthYear;
    int approxAge = 2026 - birthYear;
    std::cout << "Approximate age: " << approxAge << "\n";

    // Exercise 4
    std::cout << "\nExercise 4:\n";
    std::cout << "long:          " << sizeof(long)          << " bytes\n";
    std::cout << "long long:     " << sizeof(long long)     << " bytes\n";
    std::cout << "unsigned int:  " << sizeof(unsigned int)  << " bytes\n";

    // Exercise 5
    std::cout << "\nExercise 5:\n";
    const double SPEED_OF_LIGHT = 299792458.0;
    std::cout << "Speed of light = " << SPEED_OF_LIGHT << " m/s\n";
    // SPEED_OF_LIGHT = 0;  // Uncommenting this causes: error: assignment of read-only variable

    return 0;
}
