// Day 05 — Functions: declarations, overloading, default args, references vs pointers

#include <iostream>
#include <string>
#include <vector>

// --- Function declarations (prototypes) ---
// Declare before main so they can be used in main without defining them first.
int add(int a, int b);
double power(double base, int exp = 2);   // default argument
void greet(const std::string& name);      // const reference — efficient, read-only
void swap_by_ref(int& a, int& b);         // reference parameter — modifies caller
void swap_by_ptr(int* a, int* b);         // pointer parameter — also modifies caller
int factorial(int n);                     // recursive function
void print_vector(const std::vector<int>& v); // reference to avoid copying

// --- Function overloading ---
double area(double r);             // circle
double area(double w, double h);   // rectangle
double area(double a, double b, double c); // triangle (Heron's)

int main() {
    std::cout << "=== Day 05: Functions ===\n";

    // --- Basic call ---
    std::cout << "\n--- Basic function call ---\n";
    int result = add(3, 7);
    std::cout << "add(3, 7) = " << result << "\n";

    // --- Default arguments ---
    std::cout << "\n--- Default arguments ---\n";
    std::cout << "power(5)    = " << power(5)    << "\n";  // uses exp=2
    std::cout << "power(5, 3) = " << power(5, 3) << "\n";

    // --- const reference parameter ---
    std::cout << "\n--- const reference parameter ---\n";
    std::string name = "Alice";
    greet(name);

    // --- References vs Pointers ---
    std::cout << "\n--- swap by reference ---\n";
    int x = 10, y = 20;
    std::cout << "Before: x=" << x << " y=" << y << "\n";
    swap_by_ref(x, y);
    std::cout << "After:  x=" << x << " y=" << y << "\n";

    std::cout << "\n--- swap by pointer ---\n";
    int a = 100, b = 200;
    std::cout << "Before: a=" << a << " b=" << b << "\n";
    swap_by_ptr(&a, &b);  // must pass address
    std::cout << "After:  a=" << a << " b=" << b << "\n";

    // Key difference: references MUST be initialized and can't be reseated.
    // Pointers CAN be null and can be reassigned. Prefer references unless
    // you need the ptr-specific features.

    // --- Function overloading ---
    std::cout << "\n--- Function overloading ---\n";
    std::cout << "area(5.0)         = " << area(5.0)         << " (circle)\n";
    std::cout << "area(4.0, 6.0)    = " << area(4.0, 6.0)    << " (rectangle)\n";
    std::cout << "area(3.0, 4.0, 5.0)= "<< area(3.0, 4.0, 5.0)<< " (triangle)\n";

    // --- Recursion ---
    std::cout << "\n--- Recursion ---\n";
    for (int i = 0; i <= 10; ++i) {
        std::cout << i << "! = " << factorial(i) << "\n";
    }

    // --- Pass vector by const reference ---
    std::cout << "\n--- Pass vector by const& ---\n";
    std::vector<int> nums = {1, 2, 3, 4, 5};
    print_vector(nums);

    return 0;
}

// --- Function definitions ---

int add(int a, int b) {
    return a + b;
}

double power(double base, int exp) {
    double result = 1.0;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

void greet(const std::string& name) {
    std::cout << "Hello, " << name << "! (received by const reference)\n";
}

void swap_by_ref(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void swap_by_ptr(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

void print_vector(const std::vector<int>& v) {
    std::cout << "[ ";
    for (const int& val : v) std::cout << val << " ";
    std::cout << "]\n";
}

// Overloaded area functions
#include <cmath>
double area(double r) {
    return 3.14159265358979 * r * r;
}
double area(double w, double h) {
    return w * h;
}
double area(double a, double b, double c) {
    // Heron's formula
    double s = (a + b + c) / 2.0;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}
