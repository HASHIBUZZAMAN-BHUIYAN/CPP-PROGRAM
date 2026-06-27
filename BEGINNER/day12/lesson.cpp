// Day 12 — Exception Handling: try/catch/throw, standard exceptions, custom exceptions

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <cmath>

// --- Custom exception class ---
class DivisionByZeroException : public std::runtime_error {
public:
    DivisionByZeroException()
        : std::runtime_error("Division by zero") {}
    DivisionByZeroException(const std::string& msg)
        : std::runtime_error("Division by zero: " + msg) {}
};

class InvalidInputException : public std::invalid_argument {
    int bad_value;
public:
    InvalidInputException(int v)
        : std::invalid_argument("Invalid input: " + std::to_string(v)),
          bad_value(v) {}
    int get_bad_value() const { return bad_value; }
};

// --- Functions that throw ---
double safe_divide(double a, double b) {
    if (b == 0) throw DivisionByZeroException("b=" + std::to_string(a) + "/0");
    return a / b;
}

int square_root_int(int n) {
    if (n < 0) throw std::domain_error("sqrt of negative number: " + std::to_string(n));
    return static_cast<int>(std::sqrt(n));
}

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
}

void validate_age(int age) {
    if (age < 0 || age > 150) throw InvalidInputException(age);
}

int main() {
    std::cout << "=== Day 12: Exception Handling ===\n";

    // --- Basic try/catch ---
    std::cout << "\n--- Basic try/catch ---\n";
    try {
        double result = safe_divide(10.0, 0.0);
        std::cout << "Result: " << result << "\n";  // won't reach here
    } catch (const DivisionByZeroException& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    // --- Multiple catch blocks ---
    std::cout << "\n--- Multiple catch blocks ---\n";
    std::vector<double> divisors = {2.0, 0.0, -1.0};
    for (double d : divisors) {
        try {
            std::cout << "10 / " << d << " = " << safe_divide(10.0, d) << "\n";
        } catch (const DivisionByZeroException& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    // --- Catching base class catches all derived ---
    std::cout << "\n--- Catching std::exception (base class) ---\n";
    try {
        throw std::out_of_range("index 99 out of range");
    } catch (const std::exception& e) {
        // catches any standard exception
        std::cout << "std::exception caught: " << e.what() << "\n";
    }

    // --- Catch order matters: most derived first ---
    std::cout << "\n--- Catch order: derived before base ---\n";
    try {
        square_root_int(-5);
    } catch (const std::domain_error& e) {   // more specific
        std::cout << "domain_error: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {   // less specific
        std::cout << "runtime_error: " << e.what() << "\n";
    } catch (...) {                            // catch-all
        std::cout << "Unknown exception\n";
    }

    // --- Custom exception with extra data ---
    std::cout << "\n--- Custom exception with extra data ---\n";
    try {
        validate_age(-5);
    } catch (const InvalidInputException& e) {
        std::cout << "Invalid: " << e.what()
                  << " (value was " << e.get_bad_value() << ")\n";
    }

    // --- Exception from file I/O ---
    std::cout << "\n--- File I/O exception ---\n";
    try {
        std::string content = read_file("nonexistent_file.txt");
    } catch (const std::runtime_error& e) {
        std::cout << "File error: " << e.what() << "\n";
    }

    // --- Re-throwing ---
    std::cout << "\n--- Re-throwing ---\n";
    auto process = [](double d) {
        try {
            return safe_divide(100.0, d);
        } catch (const DivisionByZeroException&) {
            std::cout << "  (inner handler — re-throwing)\n";
            throw;  // re-throw the same exception to outer handler
        }
    };

    try {
        process(0.0);
    } catch (const DivisionByZeroException& e) {
        std::cout << "Outer caught re-thrown: " << e.what() << "\n";
    }

    // --- noexcept ---
    std::cout << "\n--- noexcept ---\n";
    // Mark functions that NEVER throw with noexcept.
    // The compiler can optimize them and containers use them for move operations.
    auto safe_add = [](int a, int b) noexcept -> int { return a + b; };
    std::cout << "safe_add(3,4) = " << safe_add(3, 4) << "\n";

    // --- Standard exception hierarchy ---
    std::cout << "\n--- Standard exception types ---\n";
    std::cout << "std::exception\n";
    std::cout << "  std::logic_error\n";
    std::cout << "    std::invalid_argument\n";
    std::cout << "    std::out_of_range\n";
    std::cout << "    std::domain_error\n";
    std::cout << "  std::runtime_error\n";
    std::cout << "    std::range_error\n";
    std::cout << "    std::overflow_error\n";
    std::cout << "  std::bad_alloc (new failure)\n";

    return 0;
}
