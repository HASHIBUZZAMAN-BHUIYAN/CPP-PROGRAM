// Day 13 — Exercises: STL

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <string>

int main() {
    std::cout << "=== Day 13 Exercises ===\n\n";

    // Exercise 1: Word frequency with map
    // TODO: Read a sentence from the user. Count the frequency of each word.
    //       Print the word and count in alphabetical order.
    std::cout << "Exercise 1 - Word frequency:\n";
    // YOUR CODE HERE


    // Exercise 2: Set operations
    // TODO: Given two sets A = {1,2,3,4,5} and B = {3,4,5,6,7}
    //       Compute and print their union, intersection, and difference (A-B).
    //       Hint: std::set_union, std::set_intersection, std::set_difference
    //       (require sorted ranges, so vector output + std::back_inserter)
    std::cout << "\nExercise 2 - Set operations:\n";
    // YOUR CODE HERE


    // Exercise 3: STL algorithms pipeline
    // TODO: Given vector<int> = {1..20}
    //       a) Filter: keep only numbers divisible by 3
    //       b) Transform: square each remaining number
    //       c) Accumulate: sum them all
    //       Print the result at each step.
    std::cout << "\nExercise 3 - Algorithm pipeline:\n";
    // YOUR CODE HERE


    // Exercise 4: Priority queue (max-heap)
    // TODO: Push numbers {3,1,4,1,5,9,2,6,5} into a std::priority_queue<int>.
    //       Pop and print them — they should come out in descending order.
    std::cout << "\nExercise 4 - Priority queue:\n";
    // YOUR CODE HERE


    // Exercise 5: Custom sort with lambda
    // TODO: struct Person { string name; int age; };
    //       Sort a vector of Persons first by age ascending, then by name alphabetically.
    //       Print the sorted list.
    std::cout << "\nExercise 5 - Custom sort:\n";
    // YOUR CODE HERE


    return 0;
}
