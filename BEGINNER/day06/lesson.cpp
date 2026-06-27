// Day 06 — Arrays, std::array, std::vector

#include <iostream>
#include <array>
#include <vector>
#include <algorithm>  // std::sort, std::reverse, std::find

int main() {
    std::cout << "=== Day 06: Arrays and Vectors ===\n";

    // ============================================================
    // PART 1: Raw C-style arrays (know these, but prefer vector)
    // ============================================================
    std::cout << "\n--- Raw arrays ---\n";
    int scores[5] = {85, 92, 78, 96, 70};

    // Access by index
    std::cout << "scores[2] = " << scores[2] << "\n";

    // Iterate with index
    std::cout << "All scores: ";
    for (int i = 0; i < 5; ++i) std::cout << scores[i] << " ";
    std::cout << "\n";

    // sizeof trick to get length (only works in the same scope, not if passed to a function)
    int len = sizeof(scores) / sizeof(scores[0]);
    std::cout << "Length via sizeof: " << len << "\n";

    // 2D raw array
    int matrix[2][3] = {{1,2,3},{4,5,6}};
    std::cout << "2D matrix:\n";
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 3; ++c) std::cout << matrix[r][c] << " ";
        std::cout << "\n";
    }

    // ============================================================
    // PART 2: std::array — fixed-size, safe, knows its own size
    // ============================================================
    std::cout << "\n--- std::array ---\n";
    std::array<int, 5> arr = {10, 20, 30, 40, 50};

    std::cout << "arr.size() = " << arr.size() << "\n";
    std::cout << "arr[2]     = " << arr[2]     << "\n";
    std::cout << "arr.at(2)  = " << arr.at(2)  << "  (bounds-checked)\n";
    // arr.at(10);  // would throw std::out_of_range

    // Range-based for
    std::cout << "arr: ";
    for (const auto& v : arr) std::cout << v << " ";
    std::cout << "\n";

    // Sort an array
    std::sort(arr.begin(), arr.end(), std::greater<int>());
    std::cout << "Sorted descending: ";
    for (const auto& v : arr) std::cout << v << " ";
    std::cout << "\n";

    // ============================================================
    // PART 3: std::vector — dynamic, resizable, THE workhorse
    // ============================================================
    std::cout << "\n--- std::vector ---\n";

    std::vector<int> v;           // starts empty
    v.push_back(3);
    v.push_back(1);
    v.push_back(4);
    v.push_back(1);
    v.push_back(5);

    std::cout << "v.size()     = " << v.size()     << "\n";
    std::cout << "v.capacity() = " << v.capacity() << " (allocated slots)\n";
    std::cout << "v.front()    = " << v.front()    << "\n";
    std::cout << "v.back()     = " << v.back()     << "\n";

    // Initialize with size and value
    std::vector<int> zeros(5, 0);
    std::cout << "zeros: ";
    for (int x : zeros) std::cout << x << " ";
    std::cout << "\n";

    // Initialize from list
    std::vector<std::string> fruits = {"apple", "banana", "cherry"};

    // push_back / pop_back
    fruits.push_back("date");
    fruits.pop_back();
    std::cout << "Fruits: ";
    for (const auto& f : fruits) std::cout << f << " ";
    std::cout << "\n";

    // insert and erase
    fruits.insert(fruits.begin() + 1, "apricot");
    fruits.erase(fruits.begin() + 2);
    std::cout << "After insert+erase: ";
    for (const auto& f : fruits) std::cout << f << " ";
    std::cout << "\n";

    // Sort and reverse
    std::sort(fruits.begin(), fruits.end());
    std::cout << "Sorted: ";
    for (const auto& f : fruits) std::cout << f << " ";
    std::cout << "\n";

    // Find
    auto it = std::find(fruits.begin(), fruits.end(), "banana");
    if (it != fruits.end()) {
        std::cout << "Found 'banana' at index " << (it - fruits.begin()) << "\n";
    }

    // 2D vector
    std::cout << "\n--- 2D vector ---\n";
    std::vector<std::vector<int>> grid = {{1,2,3},{4,5,6},{7,8,9}};
    for (const auto& row : grid) {
        for (int cell : row) std::cout << cell << " ";
        std::cout << "\n";
    }

    // ============================================================
    // Summary: which to use?
    // ============================================================
    std::cout << "\n--- When to use what ---\n";
    std::cout << "Raw array : avoid — use std::array or vector instead\n";
    std::cout << "std::array: compile-time fixed size, stack-allocated, fast\n";
    std::cout << "std::vector: runtime-resizable, heap-allocated, the default choice\n";

    return 0;
}
