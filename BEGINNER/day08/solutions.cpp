// Day 08 — Solutions

#include <iostream>
#include <vector>
#include <algorithm>

void min_max(const std::vector<int>& v, int& min_val, int& max_val) {
    min_val = max_val = v[0];
    for (int x : v) {
        if (x < min_val) min_val = x;
        if (x > max_val) max_val = x;
    }
}

void fill_array(int* arr, int size, int value) {
    for (int i = 0; i < size; ++i) arr[i] = value;
}

void swap_elements(std::vector<int>& v, int i, int j) {
    int tmp = v[i]; v[i] = v[j]; v[j] = tmp;
}

int* find_max(int* arr, int size) {
    int* maxPtr = arr;
    for (int i = 1; i < size; ++i)
        if (arr[i] > *maxPtr) maxPtr = arr + i;
    return maxPtr;
}

int main() {
    // Exercise 1
    std::cout << "Exercise 1:\n";
    int x = 42;
    int* p = &x;
    std::cout << "Address of x: " << p  << "\n";
    std::cout << "Value: " << *p << "\n";
    *p = 99;
    std::cout << "After *p=99: x=" << x << "\n";

    // Exercise 2
    std::cout << "\nExercise 2:\n";
    std::vector<int> v = {5, 3, 9, 1, 7};
    int mn, mx;
    min_max(v, mn, mx);
    std::cout << "Min=" << mn << " Max=" << mx << "\n";

    // Exercise 3
    std::cout << "\nExercise 3:\n";
    int arr[5];
    fill_array(arr, 5, 42);
    for (int i : arr) std::cout << i << " ";
    std::cout << "\n";

    // Exercise 4
    std::cout << "\nExercise 4:\n";
    std::vector<int> v2 = {1,2,3,4,5};
    swap_elements(v2, 0, 4);
    for (int x2 : v2) std::cout << x2 << " ";
    std::cout << "\n";

    // Exercise 5
    std::cout << "\nExercise 5:\n";
    int arr2[6] = {3, 9, 2, 7, 1, 5};
    int* maxP = find_max(arr2, 6);
    std::cout << "Max = " << *maxP << "\n";
    *maxP = 100;
    for (int x2 : arr2) std::cout << x2 << " ";
    std::cout << "\n";

    return 0;
}
