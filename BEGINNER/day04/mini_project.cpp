// Day 04 Mini Project — Multiplication Table Generator

#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== Multiplication Table Generator ===\n";
    std::cout << "Enter table size (e.g. 10 for 10x10): ";
    int size;
    std::cin >> size;
    if (size < 1 || size > 20) { std::cout << "Size must be 1-20.\n"; return 1; }

    std::cout << "\n";
    // Header row
    std::cout << std::setw(4) << " ";
    for (int col = 1; col <= size; ++col)
        std::cout << std::setw(5) << col;
    std::cout << "\n";
    std::cout << "    " << std::string(size * 5, '-') << "\n";

    for (int row = 1; row <= size; ++row) {
        std::cout << std::setw(3) << row << " |";
        for (int col = 1; col <= size; ++col) {
            std::cout << std::setw(5) << (row * col);
        }
        std::cout << "\n";
    }

    return 0;
}
