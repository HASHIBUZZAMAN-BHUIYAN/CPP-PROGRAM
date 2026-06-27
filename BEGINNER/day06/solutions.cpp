// Day 06 — Solutions

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

int main() {
    // Exercise 1
    std::cout << "Exercise 1:\n";
    std::cout << "How many numbers? ";
    int n; std::cin >> n;
    std::vector<int> v;
    for (int i = 0; i < n; ++i) {
        int x; std::cin >> x;
        v.push_back(x);
    }
    long long sum = 0;
    for (int x : v) sum += x;
    std::cout << "Sum = " << sum << ", Avg = " << static_cast<double>(sum)/n << "\n";

    // Exercise 2
    std::cout << "\nExercise 2:\n";
    std::vector<int> dup = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    std::sort(dup.begin(), dup.end());
    auto last = std::unique(dup.begin(), dup.end());
    dup.erase(last, dup.end());
    for (int x : dup) std::cout << x << " ";
    std::cout << "\n";

    // Exercise 3
    std::cout << "\nExercise 3:\n";
    std::vector<std::vector<int>> mat = {{1,2,3},{4,5,6},{7,8,9}};
    int diagSum = 0;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) std::cout << mat[r][c] << " ";
        std::cout << "\n";
        diagSum += mat[r][r];
    }
    std::cout << "Diagonal sum = " << diagSum << "\n";

    // Exercise 4
    std::cout << "\nExercise 4:\n";
    std::array<int, 6> arr = {5, 2, 8, 1, 9, 3};
    std::cout << "Min = " << *std::min_element(arr.begin(), arr.end()) << "\n";
    std::cout << "Max = " << *std::max_element(arr.begin(), arr.end()) << "\n";
    std::sort(arr.begin(), arr.end());
    std::cout << "Sorted: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    // Exercise 5
    std::cout << "\nExercise 5:\n";
    std::vector<int> rot = {1,2,3,4,5};
    std::cout << "Before: ";
    for (int x : rot) std::cout << x << " ";
    std::cout << "\n";
    std::rotate(rot.begin(), rot.begin() + 2, rot.end());
    std::cout << "After rotate left by 2: ";
    for (int x : rot) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
