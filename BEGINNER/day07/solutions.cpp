// Day 07 — Solutions

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

int main() {
    // Exercise 1: Palindrome
    std::cout << "Exercise 1:\n";
    std::cout << "Enter a word: ";
    std::string word; std::cin >> word;
    std::string lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    std::string rev = lower;
    std::reverse(rev.begin(), rev.end());
    std::cout << word << " is " << (lower == rev ? "" : "NOT ") << "a palindrome\n";

    // Exercise 2: Word count
    std::cout << "\nExercise 2:\n";
    std::cin.ignore();
    std::cout << "Enter a sentence: ";
    std::string line; std::getline(std::cin, line);
    std::istringstream iss(line);
    int count = 0; std::string w;
    while (iss >> w) ++count;
    std::cout << "Words: " << count << "\n";

    // Exercise 3: Replace all
    std::cout << "\nExercise 3:\n";
    std::string text = "the cat sat on the mat";
    std::string from = "at", to = "ot";
    size_t pos = 0;
    while ((pos = text.find(from, pos)) != std::string::npos) {
        text.replace(pos, from.length(), to);
        pos += to.length();
    }
    std::cout << text << "\n";

    // Exercise 4: CSV
    std::cout << "\nExercise 4:\n";
    std::string row = "Alice,30,Engineer,London";
    std::istringstream csv(row);
    std::string field;
    while (std::getline(csv, field, ','))
        std::cout << field << "\n";

    // Exercise 5: Caesar cipher
    std::cout << "\nExercise 5:\n";
    std::cout << "Message: ";
    std::string msg; std::getline(std::cin, msg);
    std::cout << "Shift (1-25): ";
    int shift; std::cin >> shift;
    shift = ((shift % 26) + 26) % 26;
    for (char& ch : msg) {
        if (ch >= 'a' && ch <= 'z') ch = 'a' + (ch - 'a' + shift) % 26;
        else if (ch >= 'A' && ch <= 'Z') ch = 'A' + (ch - 'A' + shift) % 26;
    }
    std::cout << "Encrypted: " << msg << "\n";

    return 0;
}
