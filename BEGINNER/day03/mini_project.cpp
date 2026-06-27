// Day 03 Mini Project — Number Guessing Game
// Computer picks a number; player guesses until they get it right.

#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int secret = std::rand() % 100 + 1;  // 1-100

    std::cout << "=== Number Guessing Game ===\n";
    std::cout << "I've picked a number between 1 and 100. Can you guess it?\n\n";

    int guess;
    int attempts = 0;
    const int MAX_ATTEMPTS = 7;

    while (attempts < MAX_ATTEMPTS) {
        std::cout << "Guess " << (attempts + 1) << "/" << MAX_ATTEMPTS << ": ";
        std::cin >> guess;
        ++attempts;

        if (guess < 1 || guess > 100) {
            std::cout << "Please enter a number between 1 and 100.\n";
            --attempts;  // don't count invalid guesses
        } else if (guess < secret) {
            std::cout << "Too low!\n";
        } else if (guess > secret) {
            std::cout << "Too high!\n";
        } else {
            std::cout << "Correct! You got it in " << attempts << " attempt";
            if (attempts != 1) std::cout << "s";
            std::cout << "!\n";
            return 0;
        }
    }

    std::cout << "Out of guesses! The number was " << secret << ".\n";
    return 0;
}
