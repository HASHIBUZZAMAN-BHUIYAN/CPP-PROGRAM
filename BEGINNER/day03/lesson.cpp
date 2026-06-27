// Day 03 — Conditionals: if/else, switch, ternary

#include <iostream>
#include <string>

int main() {
    std::cout << "=== Day 03: Conditionals ===\n";

    // --- if / else if / else ---
    std::cout << "\n--- if / else if / else ---\n";
    int score = 75;
    std::cout << "Score: " << score << "\n";
    if (score >= 90) {
        std::cout << "Grade: A\n";
    } else if (score >= 80) {
        std::cout << "Grade: B\n";
    } else if (score >= 70) {
        std::cout << "Grade: C\n";
    } else if (score >= 60) {
        std::cout << "Grade: D\n";
    } else {
        std::cout << "Grade: F\n";
    }

    // --- Nested if ---
    std::cout << "\n--- Nested if ---\n";
    int age = 20;
    bool hasID = true;
    if (age >= 18) {
        if (hasID) {
            std::cout << "Entry allowed.\n";
        } else {
            std::cout << "Need ID.\n";
        }
    } else {
        std::cout << "Too young.\n";
    }

    // --- Ternary operator: condition ? value_if_true : value_if_false ---
    std::cout << "\n--- Ternary operator ---\n";
    int x = 42;
    std::string parity = (x % 2 == 0) ? "even" : "odd";
    std::cout << x << " is " << parity << "\n";
    int max = (10 > 7) ? 10 : 7;
    std::cout << "max(10,7) = " << max << "\n";

    // --- switch statement ---
    std::cout << "\n--- switch ---\n";
    int day = 3;
    switch (day) {
        case 1: std::cout << "Monday\n";    break;
        case 2: std::cout << "Tuesday\n";   break;
        case 3: std::cout << "Wednesday\n"; break;
        case 4: std::cout << "Thursday\n";  break;
        case 5: std::cout << "Friday\n";    break;
        case 6: std::cout << "Saturday\n";  break;
        case 7: std::cout << "Sunday\n";    break;
        default: std::cout << "Invalid day\n"; break;
    }

    // --- switch fall-through (intentional) ---
    std::cout << "\n--- switch fall-through ---\n";
    char letter = 'A';
    switch (letter) {
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
            std::cout << letter << " is a vowel\n";
            break;
        default:
            std::cout << letter << " is a consonant\n";
            break;
    }

    // --- switch on string is NOT directly supported in C++ ---
    // use if/else chains or a map for string-based dispatch

    // --- Interactive: ask user ---
    std::cout << "\n--- Interactive ---\n";
    std::cout << "Enter a number (1-7 for day of week): ";
    int input;
    std::cin >> input;
    switch (input) {
        case 1: std::cout << "Monday — start of the work week!\n";    break;
        case 2: std::cout << "Tuesday — keep going!\n";               break;
        case 3: std::cout << "Wednesday — halfway there!\n";          break;
        case 4: std::cout << "Thursday — almost Friday!\n";           break;
        case 5: std::cout << "Friday — weekend is near!\n";           break;
        case 6: std::cout << "Saturday — enjoy the weekend!\n";       break;
        case 7: std::cout << "Sunday — rest up!\n";                   break;
        default: std::cout << "Not a valid day number.\n";            break;
    }

    // --- Common mistake: assignment in condition ---
    // if (x = 5) is ALWAYS true (assigns 5) — it should be (x == 5)
    // Turning on -Wall will warn you about this.

    return 0;
}
