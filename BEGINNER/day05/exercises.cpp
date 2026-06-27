// Day 05 — Exercises: Functions

#include <iostream>
#include <string>
#include <vector>

// Exercise 1: Declare and define a function is_prime(int n) -> bool
// TODO: Implement it here (above main)
// YOUR CODE HERE


// Exercise 2: Overloaded max function
// TODO: Write three overloaded versions of max():
//       max(int, int), max(double, double), max(std::string, std::string)
// YOUR CODE HERE


// Exercise 3: Function that reverses a string in-place using a reference
// TODO: void reverse_string(std::string& s)
// YOUR CODE HERE


// Exercise 4: Function that counts occurrences of a char in a string
// TODO: int count_char(const std::string& s, char c)
// YOUR CODE HERE


// Exercise 5: Fibonacci with recursion
// TODO: int fibonacci(int n) -> returns the nth Fibonacci number (0-indexed).
//       fibonacci(0)=0, fibonacci(1)=1, fibonacci(2)=1, fibonacci(7)=13
// YOUR CODE HERE


int main() {
    std::cout << "=== Day 05 Exercises ===\n\n";

    // Test Exercise 1
    std::cout << "Exercise 1 - is_prime:\n";
    // TODO: Print whether 2, 7, 10, 13, 100 are prime
    // YOUR CODE HERE


    // Test Exercise 2
    std::cout << "\nExercise 2 - overloaded max:\n";
    // TODO: Call all three overloads with sample values
    // YOUR CODE HERE


    // Test Exercise 3
    std::cout << "\nExercise 3 - reverse_string:\n";
    // TODO: Reverse "Hello, World!" and print
    // YOUR CODE HERE


    // Test Exercise 4
    std::cout << "\nExercise 4 - count_char:\n";
    // TODO: Count 'l' in "hello world" and print
    // YOUR CODE HERE


    // Test Exercise 5
    std::cout << "\nExercise 5 - fibonacci:\n";
    // TODO: Print fibonacci(0) through fibonacci(10)
    // YOUR CODE HERE


    return 0;
}
