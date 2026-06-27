// Day 02 — Solutions

#include <iostream>
#include <cmath>

int main() {
    std::cout << "=== Day 02 Solutions ===\n\n";

    // Exercise 1
    std::cout << "Exercise 1:\n";
    std::cout << "Enter Celsius: ";
    double celsius;
    std::cin >> celsius;
    double fahrenheit = celsius * 9.0 / 5.0 + 32.0;
    std::cout << celsius << " C = " << fahrenheit << " F\n";

    // Exercise 2
    std::cout << "\nExercise 2:\n";
    std::cout << "Enter an integer: ";
    int num;
    std::cin >> num;
    std::cout << num << " is " << (num % 2 == 0 ? "even" : "odd") << "\n";

    // Exercise 3
    std::cout << "\nExercise 3:\n";
    std::cout << "Enter side a: ";
    double a;
    std::cin >> a;
    std::cout << "Enter side b: ";
    double b;
    std::cin >> b;
    double c = std::sqrt(a * a + b * b);
    std::cout << "Hypotenuse = " << c << "\n";

    // Exercise 4
    std::cout << "\nExercise 4:\n";
    int x = 255;
    char ch = static_cast<char>(x);
    std::cout << "int 255 as char: " << ch << "\n";
    double d = 9.99;
    int truncated = static_cast<int>(d);
    std::cout << "double " << d << " cast to int: " << truncated << "\n";

    // Exercise 5
    std::cout << "\nExercise 5:\n";
    for (int n = 0; n <= 10; ++n) {
        std::cout << "2^" << n << " = " << (1 << n) << "\n";
    }

    return 0;
}
