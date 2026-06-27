// Day 04 — Loops: for, while, do-while, range-based for, break/continue

#include <iostream>
#include <vector>
#include <string>

int main() {
    std::cout << "=== Day 04: Loops ===\n";

    // --- for loop ---
    std::cout << "\n--- for loop ---\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "i = " << i << "\n";
    }

    // Counting down
    std::cout << "Countdown: ";
    for (int i = 5; i >= 1; --i) {
        std::cout << i << " ";
    }
    std::cout << "Go!\n";

    // Step > 1
    std::cout << "Evens: ";
    for (int i = 0; i <= 20; i += 2) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // --- while loop ---
    std::cout << "\n--- while loop ---\n";
    int n = 1;
    while (n <= 10) {
        std::cout << n << " ";
        n *= 2;  // 1, 2, 4, 8
    }
    std::cout << "\n";

    // while with user input validation
    std::cout << "Enter a positive number: ";
    int input;
    std::cin >> input;
    while (input <= 0) {
        std::cout << "Must be positive. Try again: ";
        std::cin >> input;
    }
    std::cout << "Got: " << input << "\n";

    // --- do-while (runs AT LEAST once) ---
    std::cout << "\n--- do-while ---\n";
    int count = 0;
    do {
        std::cout << "Loop body executed (count=" << count << ")\n";
        ++count;
    } while (count < 3);

    // Common use: menu loop
    std::cout << "Enter 'q' to quit: ";
    char ch;
    do {
        std::cin >> ch;
        if (ch != 'q') std::cout << "You typed '" << ch << "'. Enter 'q' to quit: ";
    } while (ch != 'q');
    std::cout << "Quit!\n";

    // --- break and continue ---
    std::cout << "\n--- break ---\n";
    for (int i = 0; i < 10; ++i) {
        if (i == 5) break;   // stop the loop
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "--- continue ---\n";
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) continue;  // skip even numbers
        std::cout << i << " ";
    }
    std::cout << "\n";

    // --- range-based for (C++11) — the modern way to iterate collections ---
    std::cout << "\n--- range-based for ---\n";
    std::vector<int> numbers = {10, 20, 30, 40, 50};
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // Use const& to avoid copying (important for large objects)
    std::vector<std::string> words = {"hello", "world", "C++", "is", "fun"};
    for (const std::string& w : words) {
        std::cout << w << " ";
    }
    std::cout << "\n";

    // Auto type deduction with range-for
    for (auto& num : numbers) {
        num *= 2;  // modifies in-place (needs & not const&)
    }
    std::cout << "After doubling: ";
    for (auto n2 : numbers) std::cout << n2 << " ";
    std::cout << "\n";

    // --- Nested loops ---
    std::cout << "\n--- Nested loops: multiplication table ---\n";
    for (int row = 1; row <= 5; ++row) {
        for (int col = 1; col <= 5; ++col) {
            std::cout << (row * col) << "\t";
        }
        std::cout << "\n";
    }

    // --- Infinite loop with break ---
    std::cout << "\n--- while(true) + break ---\n";
    int attempt = 0;
    while (true) {
        ++attempt;
        std::cout << "Attempt " << attempt << "\n";
        if (attempt >= 3) break;
    }

    return 0;
}
