// Day 02 — Operators, Expressions, Type Casting, const

#include <iostream>
#include <cmath>   // sqrt, pow, abs

int main() {
    std::cout << "=== Day 02: Operators, Expressions, Type Casting ===\n";

    // --- Arithmetic operators ---
    std::cout << "\n--- Arithmetic ---\n";
    int a = 17, b = 5;
    std::cout << a << " + " << b << " = " << a + b << "\n";
    std::cout << a << " - " << b << " = " << a - b << "\n";
    std::cout << a << " * " << b << " = " << a * b << "\n";
    std::cout << a << " / " << b << " = " << a / b << "  (integer division!)\n";
    std::cout << a << " % " << b << " = " << a % b << "  (remainder)\n";

    // --- Integer division gotcha ---
    std::cout << "\n--- Integer vs floating-point division ---\n";
    std::cout << "7 / 2   = " << 7 / 2   << "  (int division, truncates)\n";
    std::cout << "7.0 / 2 = " << 7.0 / 2 << "  (double division)\n";
    std::cout << "7 / 2.0 = " << 7 / 2.0 << "  (double division)\n";

    // --- Compound assignment ---
    std::cout << "\n--- Compound assignment ---\n";
    int x = 10;
    x += 5;  std::cout << "x += 5 -> " << x << "\n";
    x -= 3;  std::cout << "x -= 3 -> " << x << "\n";
    x *= 2;  std::cout << "x *= 2 -> " << x << "\n";
    x /= 4;  std::cout << "x /= 4 -> " << x << "\n";
    x %= 3;  std::cout << "x %= 3 -> " << x << "\n";

    // --- Increment / Decrement ---
    std::cout << "\n--- Increment/Decrement ---\n";
    int n = 5;
    std::cout << "n   = " << n   << "\n";
    std::cout << "n++ = " << n++ << "  (post: returns THEN increments)\n";
    std::cout << "n   = " << n   << "\n";
    std::cout << "++n = " << ++n << "  (pre: increments THEN returns)\n";

    // --- Relational and logical operators ---
    std::cout << "\n--- Relational & Logical ---\n";
    int p = 10, q = 20;
    std::cout << std::boolalpha;
    std::cout << "p < q:   " << (p < q)  << "\n";
    std::cout << "p == q:  " << (p == q) << "\n";
    std::cout << "p != q:  " << (p != q) << "\n";
    std::cout << "(p < q) && (q < 100): " << ((p < q) && (q < 100)) << "\n";
    std::cout << "(p > q) || (q > 10):  " << ((p > q) || (q > 10))  << "\n";
    std::cout << "!(p == q):            " << !(p == q)               << "\n";

    // --- Bitwise operators (brief intro) ---
    std::cout << "\n--- Bitwise ---\n";
    int u = 0b1010, v = 0b1100;  // binary literals (C++14+)
    std::cout << "u & v  = " << (u & v)  << "  (AND)\n";
    std::cout << "u | v  = " << (u | v)  << "  (OR)\n";
    std::cout << "u ^ v  = " << (u ^ v)  << "  (XOR)\n";
    std::cout << "u << 1 = " << (u << 1) << "  (left shift = multiply by 2)\n";
    std::cout << "u >> 1 = " << (u >> 1) << "  (right shift = divide by 2)\n";

    // --- Type casting ---
    std::cout << "\n--- Type Casting ---\n";
    double pi = 3.14159;
    int truncated = (int)pi;            // C-style cast (works, but avoid in C++)
    int truncated2 = static_cast<int>(pi); // C++ style cast (preferred)
    std::cout << "pi = " << pi << "\n";
    std::cout << "(int)pi             = " << truncated  << "\n";
    std::cout << "static_cast<int>(pi)= " << truncated2 << "\n";

    // widening (safe, implicit)
    int whole = 7;
    double asDouble = whole;   // int -> double, no cast needed
    std::cout << "int 7 -> double: " << asDouble << "\n";

    // explicit cast to avoid integer division
    int aa = 7, bb = 2;
    double result = static_cast<double>(aa) / bb;
    std::cout << "7 / 2 (with cast) = " << result << "\n";

    // --- cmath functions ---
    std::cout << "\n--- <cmath> ---\n";
    std::cout << "sqrt(16) = " << std::sqrt(16.0) << "\n";
    std::cout << "pow(2,10)= " << std::pow(2.0, 10.0) << "\n";
    std::cout << "abs(-42) = " << std::abs(-42) << "\n";

    // --- const ---
    std::cout << "\n--- const ---\n";
    const int MAX_SIZE = 100;
    const double GRAVITY = 9.81;
    std::cout << "MAX_SIZE = " << MAX_SIZE << "\n";
    std::cout << "GRAVITY  = " << GRAVITY  << " m/s^2\n";
    // MAX_SIZE = 200;  // error: assignment of read-only variable

    return 0;
}
