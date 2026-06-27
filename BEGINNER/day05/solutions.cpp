// Day 05 — Solutions

#include <iostream>
#include <string>
#include <algorithm>

bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

int max_int(int a, int b) { return a > b ? a : b; }
double max_double(double a, double b) { return a > b ? a : b; }
std::string max_string(const std::string& a, const std::string& b) { return a > b ? a : b; }

void reverse_string(std::string& s) {
    std::reverse(s.begin(), s.end());
}

int count_char(const std::string& s, char c) {
    int count = 0;
    for (char ch : s) if (ch == c) ++count;
    return count;
}

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    std::cout << "=== Day 05 Solutions ===\n\n";

    std::cout << "Exercise 1:\n";
    for (int n : {2, 7, 10, 13, 100})
        std::cout << n << " is " << (is_prime(n) ? "" : "not ") << "prime\n";

    std::cout << "\nExercise 2:\n";
    std::cout << "max(3, 7)         = " << max_int(3, 7)               << "\n";
    std::cout << "max(3.5, 2.1)     = " << max_double(3.5, 2.1)        << "\n";
    std::cout << "max(banana,apple) = " << max_string("banana","apple") << "\n";

    std::cout << "\nExercise 3:\n";
    std::string s = "Hello, World!";
    reverse_string(s);
    std::cout << "Reversed: " << s << "\n";

    std::cout << "\nExercise 4:\n";
    std::cout << "Count of 'l' in \"hello world\" = "
              << count_char("hello world", 'l') << "\n";

    std::cout << "\nExercise 5:\n";
    for (int i = 0; i <= 10; ++i)
        std::cout << "fib(" << i << ") = " << fibonacci(i) << "\n";

    return 0;
}
