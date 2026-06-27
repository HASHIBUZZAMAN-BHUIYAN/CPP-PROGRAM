// Day 05 Mini Project — Simple Math Library
// Demonstrates functions, overloading, and default arguments.

#include <iostream>
#include <cmath>
#include <string>

double add(double a, double b)      { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b)   { return b != 0 ? a / b : 0; }
double power(double base, double exp = 2.0) { return std::pow(base, exp); }
double circle_area(double r)        { return 3.14159265358979 * r * r; }
double rect_area(double w, double h){ return w * h; }

bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) if (n % i == 0) return false;
    return true;
}

long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

void run_demo() {
    double a = 15.0, b = 4.0;
    std::cout << "a=" << a << ", b=" << b << "\n";
    std::cout << "  add:      " << add(a, b)      << "\n";
    std::cout << "  subtract: " << subtract(a, b) << "\n";
    std::cout << "  multiply: " << multiply(a, b) << "\n";
    std::cout << "  divide:   " << divide(a, b)   << "\n";
    std::cout << "  power(a): " << power(a)       << " (default exp=2)\n";
    std::cout << "  power(2,10): " << power(2, 10) << "\n";
    std::cout << "  circle_area(5):   " << circle_area(5.0)    << "\n";
    std::cout << "  rect_area(4,6):   " << rect_area(4.0, 6.0) << "\n";

    std::cout << "\nPrimes up to 30: ";
    for (int i = 2; i <= 30; ++i) if (is_prime(i)) std::cout << i << " ";
    std::cout << "\n";

    std::cout << "Factorials 1-10:\n";
    for (int i = 1; i <= 10; ++i)
        std::cout << "  " << i << "! = " << factorial(i) << "\n";
}

int main() {
    std::cout << "=== Simple Math Library Demo ===\n\n";
    run_demo();
    return 0;
}
