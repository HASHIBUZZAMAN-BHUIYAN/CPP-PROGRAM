// Day 07 — Exercises: Strings

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

int main() {
    std::cout << "=== Day 07 Exercises ===\n\n";

    // Exercise 1: Palindrome check
    // TODO: Ask the user for a word. Check if it's a palindrome
    //       (reads the same forwards and backwards, case-insensitive).
    //       Hint: convert to lowercase, then compare s == reversed(s).
    std::cout << "Exercise 1 - Palindrome:\n";
    // YOUR CODE HERE


    // Exercise 2: Word counter
    // TODO: Ask the user to type a sentence.
    //       Count and print the number of words (split by whitespace).
    std::cout << "\nExercise 2 - Word count:\n";
    // YOUR CODE HERE


    // Exercise 3: String replace-all
    // TODO: Given string text = "the cat sat on the mat";
    //       Replace every "at" with "ot" and print the result.
    //       std::string::replace only replaces one occurrence — loop it.
    std::cout << "\nExercise 3 - Replace all:\n";
    // YOUR CODE HERE


    // Exercise 4: CSV parser
    // TODO: Given string row = "Alice,30,Engineer,London";
    //       Split by ',' into a vector<string> using getline on an istringstream.
    //       Print each field on its own line.
    std::cout << "\nExercise 4 - CSV parse:\n";
    // YOUR CODE HERE


    // Exercise 5: String encryption (Caesar cipher)
    // TODO: Ask for a message and a shift value (1-25).
    //       Encrypt by shifting each letter by 'shift' positions.
    //       Only shift a-z and A-Z; leave other characters unchanged.
    std::cout << "\nExercise 5 - Caesar cipher:\n";
    // YOUR CODE HERE


    return 0;
}
