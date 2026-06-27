// Day 01 — C++ Basics
// Covers: #include, main(), cout/cin, variables, primitive types, namespaces

#include <iostream>
#include <string>
#include <climits>   // INT_MAX, INT_MIN, etc.

int main() {
    std::cout << "=== Day 01: C++ Basics ===\n";

    // --- Primitive types ---
    int age = 25;
    double height = 1.78;       // 64-bit float — prefer double over float
    float temperature = 36.6f;  // f suffix required for float literals
    char grade = 'A';
    bool isStudent = true;

    std::cout << "\n--- Primitive Types ---\n";
    std::cout << "int    age         = " << age         << "\n";
    std::cout << "double height      = " << height      << " m\n";
    std::cout << "float  temperature = " << temperature << " C\n";
    std::cout << "char   grade       = " << grade       << "\n";
    std::cout << "bool   isStudent   = " << std::boolalpha << isStudent << "\n";

    // --- std::string ---
    std::string name = "Alice";
    std::cout << "\nHello, " << name << "!\n";

    // --- sizeof: how many bytes each type occupies ---
    std::cout << "\n--- sizeof each type ---\n";
    std::cout << "int:    " << sizeof(int)    << " bytes\n";
    std::cout << "double: " << sizeof(double) << " bytes\n";
    std::cout << "float:  " << sizeof(float)  << " bytes\n";
    std::cout << "char:   " << sizeof(char)   << " bytes\n";
    std::cout << "bool:   " << sizeof(bool)   << " bytes\n";

    // --- Integer limits from <climits> ---
    std::cout << "\n--- Integer limits ---\n";
    std::cout << "INT_MAX = " << INT_MAX << "\n";
    std::cout << "INT_MIN = " << INT_MIN << "\n";

    // --- Constants ---
    const double PI = 3.14159265358979;
    std::cout << "\nPI = " << PI << "\n";

    // --- User input with >> ---
    std::cout << "\n--- User Input ---\n";
    std::cout << "Enter your name: ";
    std::string userName;
    std::cin >> userName;

    std::cout << "Enter your age: ";
    int userAge;
    std::cin >> userAge;

    std::cout << "Hello " << userName << ", you are " << userAge << " years old.\n";

    // --- Multiple values on one line ---
    std::cout << "Enter two integers (space-separated): ";
    int a, b;
    std::cin >> a >> b;
    std::cout << "Sum = " << a + b << "\n";

    // --- getline for sentences (strings with spaces) ---
    std::cin.ignore(); // flush the '\n' left in the buffer after >> b
    std::cout << "Enter a full sentence: ";
    std::string sentence;
    std::getline(std::cin, sentence);
    std::cout << "You typed: " << sentence << "\n";

    // --- Namespace note ---
    // "using namespace std;" lets you write cout instead of std::cout.
    // It's fine in small practice files, but avoid it in headers and
    // large projects — it can silently pull in unexpected names.

    return 0;  // returning 0 signals success to the OS
}
