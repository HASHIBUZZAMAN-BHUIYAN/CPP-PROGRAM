// Day 02 Mini Project — Unit Converter
// Converts between common units using arithmetic and type casting.

#include <iostream>
#include <cmath>

int main() {
    std::cout << "=== Unit Converter ===\n\n";
    std::cout << "Select conversion:\n";
    std::cout << "  1. Celsius <-> Fahrenheit\n";
    std::cout << "  2. Kilometers <-> Miles\n";
    std::cout << "  3. Kilograms <-> Pounds\n";
    std::cout << "Choice (1-3): ";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "  a. C to F\n  b. F to C\nDirection: ";
        char dir;
        std::cin >> dir;
        std::cout << "Enter value: ";
        double val;
        std::cin >> val;
        if (dir == 'a' || dir == 'A')
            std::cout << val << " C = " << (val * 9.0/5.0 + 32.0) << " F\n";
        else
            std::cout << val << " F = " << ((val - 32.0) * 5.0/9.0) << " C\n";
    } else if (choice == 2) {
        std::cout << "  a. km to miles\n  b. miles to km\nDirection: ";
        char dir;
        std::cin >> dir;
        std::cout << "Enter value: ";
        double val;
        std::cin >> val;
        const double KM_PER_MILE = 1.60934;
        if (dir == 'a' || dir == 'A')
            std::cout << val << " km = " << (val / KM_PER_MILE) << " miles\n";
        else
            std::cout << val << " miles = " << (val * KM_PER_MILE) << " km\n";
    } else if (choice == 3) {
        std::cout << "  a. kg to lbs\n  b. lbs to kg\nDirection: ";
        char dir;
        std::cin >> dir;
        std::cout << "Enter value: ";
        double val;
        std::cin >> val;
        const double LBS_PER_KG = 2.20462;
        if (dir == 'a' || dir == 'A')
            std::cout << val << " kg = " << (val * LBS_PER_KG) << " lbs\n";
        else
            std::cout << val << " lbs = " << (val / LBS_PER_KG) << " kg\n";
    } else {
        std::cout << "Invalid choice.\n";
    }

    return 0;
}
