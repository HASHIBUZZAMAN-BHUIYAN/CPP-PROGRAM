// Day 08 — Pointers & References

#include <iostream>
#include <string>
#include <vector>

void demo_references();
void demo_pointers();
void demo_pointer_arithmetic();
void demo_pointer_to_array();
void demo_nullptr();

int main() {
    std::cout << "=== Day 08: Pointers & References ===\n";
    demo_references();
    demo_pointers();
    demo_pointer_arithmetic();
    demo_pointer_to_array();
    demo_nullptr();
    return 0;
}

void demo_references() {
    std::cout << "\n--- References ---\n";

    int x = 10;
    int& ref = x;   // ref IS x — not a copy, not a pointer, same memory

    std::cout << "x   = " << x   << "\n";
    std::cout << "ref = " << ref << "\n";

    ref = 99;  // changing ref changes x
    std::cout << "After ref = 99: x = " << x << "\n";

    // References must be initialized when declared (can't be null, can't change target)
    // int& bad;  // error: uninitialized reference

    // const reference — read-only alias, can bind to temporaries
    const int& cref = x;
    // cref = 0;  // error: read-only
    std::cout << "const ref: " << cref << "\n";
}

void demo_pointers() {
    std::cout << "\n--- Pointers ---\n";

    int x = 42;
    int* p = &x;  // p holds the ADDRESS of x

    std::cout << "x        = " << x  << "\n";
    std::cout << "&x       = " << &x << "  (address of x)\n";
    std::cout << "p        = " << p  << "  (p stores that address)\n";
    std::cout << "*p       = " << *p << "  (dereference: value at address)\n";

    *p = 100;  // change the value AT the address
    std::cout << "After *p = 100: x = " << x << "\n";

    // Pointer to pointer
    int** pp = &p;
    std::cout << "**pp = " << **pp << "  (double dereference)\n";

    // const pointer vs pointer to const
    const int ci = 5;
    const int* pc = &ci;  // pointer to const — can't change *pc
    // *pc = 6;  // error

    int y = 7;
    int* const cp = &y;  // const pointer — can't change where it points
    *cp = 8;             // ok — can change the value
    // cp = &x;          // error — can't reseat

    std::cout << "*pc = " << *pc << "  (pointer to const)\n";
    std::cout << "*cp = " << *cp << "  (const pointer)\n";
}

void demo_pointer_arithmetic() {
    std::cout << "\n--- Pointer arithmetic ---\n";

    int arr[5] = {10, 20, 30, 40, 50};
    int* p = arr;  // points to first element

    std::cout << "p[0] = " << *p     << "\n";
    std::cout << "p[1] = " << *(p+1) << "  (p+1 moves one int forward)\n";
    std::cout << "p[2] = " << *(p+2) << "\n";

    // Pointer arithmetic advances by sizeof(T)
    // p + 1 actually moves the address by sizeof(int) bytes

    for (int i = 0; i < 5; ++i) {
        std::cout << *(p + i) << " ";
    }
    std::cout << "\n";
}

void demo_pointer_to_array() {
    std::cout << "\n--- Array/Pointer relationship ---\n";
    // An array name DECAYS to a pointer to its first element
    int nums[] = {1, 2, 3, 4, 5};
    int* p = nums;  // no & needed — array already decays

    std::cout << "nums[2]  = " << nums[2]  << "\n";
    std::cout << "*(p+2)   = " << *(p+2)   << "\n";
    std::cout << "p[2]     = " << p[2]     << "  (pointer subscript = nums[2])\n";
}

void demo_nullptr() {
    std::cout << "\n--- nullptr (C++11) ---\n";
    int* p = nullptr;  // explicitly null — use this, NOT NULL or 0

    if (p == nullptr) {
        std::cout << "p is null — safe to check before dereferencing\n";
    }

    // Dereferencing nullptr is UNDEFINED BEHAVIOUR — always check before using
    int x = 5;
    p = &x;
    if (p != nullptr) {
        std::cout << "*p = " << *p << "\n";
    }

    // References vs Pointers — summary
    std::cout << "\n--- Ref vs Ptr summary ---\n";
    std::cout << "Reference: must initialize, can't be null, can't reseat, nicer syntax\n";
    std::cout << "Pointer:   can be null, can be reassigned, can do arithmetic\n";
    std::cout << "In modern C++: prefer references; use smart ptrs for ownership\n";
}
