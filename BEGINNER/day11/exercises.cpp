// Day 11 — Exercises: Operator Overloading & Rule of Three

#include <iostream>
#include <string>
#include <cstring>

// Exercise 1: Matrix2x2 class
// TODO: Implement a 2x2 matrix class:
//   private: double data[2][2]
//   public: Matrix2x2(initializer values), operator+, operator*, operator<<
//           determinant() const
// YOUR CODE HERE


// Exercise 2: String class with Rule of Three
// TODO: Implement MyString with:
//   - raw char* data member
//   - Constructor, Destructor, Copy Constructor, Copy Assignment
//   - operator+= (append), operator== (compare), operator<< (print)
// YOUR CODE HERE


// Exercise 3: Complex number class
// TODO: Implement Complex with double real, imag:
//   - operator+, operator-, operator*, operator==, operator<<
//   - magnitude() const
// YOUR CODE HERE


int main() {
    std::cout << "=== Day 11 Exercises ===\n\n";

    std::cout << "Exercise 1 - Matrix2x2:\n";
    // TODO: Test matrix addition and multiplication
    // YOUR CODE HERE


    std::cout << "\nExercise 2 - MyString Rule of Three:\n";
    // TODO: Create "hello", copy it, assign "world", concatenate, compare
    // YOUR CODE HERE


    std::cout << "\nExercise 3 - Complex numbers:\n";
    // TODO: (3+4i) + (1+2i), (3+4i) * (1+2i), magnitude of (3+4i)
    // YOUR CODE HERE


    return 0;
}
