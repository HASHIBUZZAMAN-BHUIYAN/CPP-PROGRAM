// Day 08 — Exercises: Pointers & References

#include <iostream>
#include <string>
#include <vector>

// Exercise 3 helper — implement this
void fill_array(int* arr, int size, int value);

// Exercise 4 — implement this: swap elements at indices i and j in the vector
void swap_elements(std::vector<int>& v, int i, int j);

// Exercise 5 — implement this
int* find_max(int* arr, int size);

int main() {
    std::cout << "=== Day 08 Exercises ===\n\n";

    // Exercise 1: Address and dereference
    // TODO: Declare int x = 42.
    //       Print the address of x (using &) and its value (using *p where p = &x).
    //       Change x to 99 using the pointer only (not x directly). Print again.
    std::cout << "Exercise 1 - Address/dereference:\n";
    // YOUR CODE HERE


    // Exercise 2: reference as output parameter
    // TODO: Write a function (inline or below main) called min_max that takes
    //       a vector<int> and two int& parameters (min_val, max_val).
    //       It should set min_val and max_val to the min/max of the vector.
    //       Call it with {5, 3, 9, 1, 7} and print both.
    std::cout << "\nExercise 2 - min_max via references:\n";
    // YOUR CODE HERE


    // Exercise 3: Pointer parameter
    // TODO: Call fill_array with an int[5] array to fill it with 42.
    //       Print all elements.
    std::cout << "\nExercise 3 - fill_array via pointer:\n";
    // YOUR CODE HERE


    // Exercise 4: Reference to vector
    // TODO: Create vector<int> v = {1,2,3,4,5}.
    //       Call swap_elements(v, 0, 4) to swap first and last.
    //       Print result.
    std::cout << "\nExercise 4 - swap via reference:\n";
    // YOUR CODE HERE


    // Exercise 5: Pointer to max element
    // TODO: int arr[6] = {3, 9, 2, 7, 1, 5};
    //       Call find_max, get the pointer, print *it and change the max to 100.
    //       Print the whole array to confirm.
    std::cout << "\nExercise 5 - Pointer to max:\n";
    // YOUR CODE HERE


    return 0;
}

void fill_array(int* arr, int size, int value) {
    // TODO: Implement
}

void swap_elements(std::vector<int>& v, int i, int j) {
    // TODO: Implement
}

int* find_max(int* arr, int size) {
    // TODO: Implement — return pointer to largest element
    return nullptr;
}
