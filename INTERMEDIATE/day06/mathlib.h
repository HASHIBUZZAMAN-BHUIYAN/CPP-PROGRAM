#pragma once
#include <cmath>

namespace mathlib {

inline int add(int a, int b) { return a + b; }
inline int multiply(int a, int b) { return a * b; }

inline long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

inline bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

inline double circle_area(double r) {
    return 3.14159265358979 * r * r;
}

} // namespace mathlib
