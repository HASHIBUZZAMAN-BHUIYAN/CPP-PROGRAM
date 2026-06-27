// Day 11 Mini Project — Calculator with History (uses operator overloading)

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

struct Calc {
    double value;
    Calc(double v = 0) : value(v) {}
    Calc operator+(double v) const { return Calc(value + v); }
    Calc operator-(double v) const { return Calc(value - v); }
    Calc operator*(double v) const { return Calc(value * v); }
    Calc operator/(double v) const {
        if (v == 0) throw std::invalid_argument("Division by zero");
        return Calc(value / v);
    }
    Calc& operator+=(double v) { value += v; return *this; }
    Calc& operator-=(double v) { value -= v; return *this; }
    Calc& operator*=(double v) { value *= v; return *this; }
    Calc& operator/=(double v) {
        if (v == 0) throw std::invalid_argument("Division by zero");
        value /= v; return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const Calc& c) {
        return os << c.value;
    }
};

int main() {
    std::cout << "=== Calculator with History ===\n";
    std::cout << "Commands: + n, - n, * n, / n, reset, history, quit\n\n";

    Calc result(0);
    std::vector<std::string> history;

    std::string cmd;
    while (true) {
        std::cout << "Current = " << result << "  > ";
        std::cin >> cmd;
        if (cmd == "quit") break;
        if (cmd == "reset") {
            result = Calc(0);
            history.push_back("reset -> 0");
            continue;
        }
        if (cmd == "history") {
            for (const auto& h : history) std::cout << "  " << h << "\n";
            continue;
        }
        double n;
        if (!(std::cin >> n)) { std::cin.clear(); std::cin.ignore(); continue; }
        try {
            double before = result.value;
            if (cmd == "+") result += n;
            else if (cmd == "-") result -= n;
            else if (cmd == "*") result *= n;
            else if (cmd == "/") result /= n;
            else { std::cout << "Unknown command\n"; continue; }
            history.push_back(std::to_string(before) + " " + cmd + " " + std::to_string(n)
                              + " = " + std::to_string(result.value));
            std::cout << " = " << result << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
    std::cout << "Final result: " << result << "\n";
    return 0;
}
