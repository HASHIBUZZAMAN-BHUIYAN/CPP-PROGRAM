// Day 12 — Exercises: Exception Handling

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

// Exercise 1: Custom exception hierarchy
// TODO: Create:
//   - class AppException : public std::runtime_error (base)
//   - class NetworkException : public AppException
//   - class TimeoutException : public NetworkException
// Throw TimeoutException and catch at each level to show the chain.
// YOUR CODE HERE


// Exercise 2: Safe vector access
// TODO: Write a function safe_get(const vector<T>& v, int idx)
//       that throws std::out_of_range with a descriptive message
//       if idx < 0 or idx >= size, otherwise returns v[idx].
// YOUR CODE HERE


int main() {
    std::cout << "=== Day 12 Exercises ===\n\n";

    // Exercise 1
    std::cout << "Exercise 1 - Exception chain:\n";
    // YOUR CODE HERE


    // Exercise 2
    std::cout << "\nExercise 2 - safe_get:\n";
    // TODO: Test safe_get with valid and invalid indices
    // YOUR CODE HERE


    // Exercise 3: Integer parsing
    // TODO: Write a function int parse_int(const string& s) that:
    //       - throws std::invalid_argument if s contains non-digits
    //       - throws std::out_of_range if the value doesn't fit in int
    //       Test with "42", "abc", "99999999999"
    std::cout << "\nExercise 3 - parse_int:\n";
    // YOUR CODE HERE


    // Exercise 4: RAII file wrapper
    // TODO: Implement class SafeFile that:
    //       - opens a file in its constructor (throws runtime_error if it fails)
    //       - closes the file in its destructor
    //       - has a read_line() method
    //       Test by trying to open a non-existent file.
    std::cout << "\nExercise 4 - RAII file:\n";
    // YOUR CODE HERE


    // Exercise 5: noexcept
    // TODO: Write a function int safe_abs(int n) noexcept that returns |n|.
    //       Write a test that calls it with INT_MIN (note: undefined behavior in real C++)
    //       and a normal value.
    std::cout << "\nExercise 5 - noexcept:\n";
    // YOUR CODE HERE


    return 0;
}
