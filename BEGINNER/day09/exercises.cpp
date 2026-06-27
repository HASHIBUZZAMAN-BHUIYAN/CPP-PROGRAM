// Day 09 — Exercises: OOP I

#include <iostream>
#include <string>
#include <vector>

// Exercise 1: Student class
// TODO: Define a Student class with:
//   private: string name, int age, vector<double> grades
//   public: constructor, add_grade(), average(), print()
// YOUR CODE HERE


// Exercise 2: Counter class
// TODO: Define a Counter class:
//   private: int value (default 0), int step
//   public: Counter(int step=1), increment(), decrement(), reset(), get()
// YOUR CODE HERE


// Exercise 3: Time class
// TODO: Define a Time class for HH:MM:SS
//   private: int hours, minutes, seconds
//   public: Time(int h, int m, int s), add_seconds(int s),
//           to_string() -> "HH:MM:SS", print()
// YOUR CODE HERE


// Exercise 4: Static member
// TODO: Add a static counter to any of the above classes to count
//       how many instances have been created. Test it by creating 3 objects.
// YOUR CODE HERE (or use an existing class)


int main() {
    std::cout << "=== Day 09 Exercises ===\n\n";

    std::cout << "Exercise 1 - Student:\n";
    // TODO: Create a Student, add grades 85, 92, 78, print them
    // YOUR CODE HERE


    std::cout << "\nExercise 2 - Counter:\n";
    // TODO: Create Counter(2), increment 5 times, decrement once, print, reset, print
    // YOUR CODE HERE


    std::cout << "\nExercise 3 - Time:\n";
    // TODO: Create Time(1, 30, 45), add 3600+90 seconds, print result
    // YOUR CODE HERE


    std::cout << "\nExercise 4 - Static count:\n";
    // TODO: Show the static count test
    // YOUR CODE HERE


    return 0;
}
