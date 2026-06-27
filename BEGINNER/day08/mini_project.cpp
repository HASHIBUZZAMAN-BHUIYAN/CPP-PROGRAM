// Day 08 Mini Project — Dynamic Array (manual resizable array using raw pointers)
// This is how vectors work under the hood — educational, not for production use.

#include <iostream>
#include <algorithm>

class DynamicArray {
private:
    int*  data;
    int   size;
    int   capacity;

    void grow() {
        int newCap = capacity == 0 ? 1 : capacity * 2;
        int* newData = new int[newCap];
        for (int i = 0; i < size; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCap;
    }

public:
    DynamicArray() : data(nullptr), size(0), capacity(0) {}

    ~DynamicArray() { delete[] data; }

    void push_back(int val) {
        if (size == capacity) grow();
        data[size++] = val;
    }

    int  get(int idx)      const { return data[idx]; }
    void set(int idx, int v)     { data[idx] = v; }
    int  length()          const { return size; }
    int  get_capacity()    const { return capacity; }

    void print() const {
        std::cout << "[";
        for (int i = 0; i < size; ++i) {
            std::cout << data[i];
            if (i + 1 < size) std::cout << ", ";
        }
        std::cout << "] size=" << size << " cap=" << capacity << "\n";
    }

    void sort_asc() { std::sort(data, data + size); }
};

int main() {
    std::cout << "=== Dynamic Array (raw pointer demo) ===\n\n";

    DynamicArray arr;
    for (int i = 1; i <= 10; ++i) {
        arr.push_back(i * 3);
        std::cout << "After push_back(" << i*3 << "): ";
        arr.print();
    }

    std::cout << "\nSorted descending:\n";
    arr.sort_asc();
    arr.print();

    std::cout << "\nThis is exactly how std::vector works internally.\n";
    std::cout << "In real code, always use std::vector — it's correct, safe, and fast.\n";

    return 0;
}
