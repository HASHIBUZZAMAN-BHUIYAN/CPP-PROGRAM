// Day 07 — Strings: std::string methods, concatenation, parsing

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

int main() {
    std::cout << "=== Day 07: std::string ===\n";

    // --- Construction ---
    std::cout << "\n--- Construction ---\n";
    std::string s1 = "Hello";
    std::string s2("World");
    std::string s3(5, '*');        // "***** "
    std::string s4 = s1;           // copy
    std::cout << s1 << " " << s2 << " " << s3 << " " << s4 << "\n";

    // --- Length and capacity ---
    std::cout << "\n--- Size ---\n";
    std::string sentence = "C++ is powerful";
    std::cout << "length(): " << sentence.length() << "\n";
    std::cout << "size():   " << sentence.size()   << "\n";  // same as length
    std::cout << "empty():  " << std::boolalpha << sentence.empty() << "\n";
    std::cout << "\"\".empty() = " << std::string().empty() << "\n";

    // --- Access ---
    std::cout << "\n--- Character access ---\n";
    std::cout << "sentence[0]    = " << sentence[0]       << "\n";
    std::cout << "sentence.at(0) = " << sentence.at(0)    << " (bounds-checked)\n";
    std::cout << "front()        = " << sentence.front()  << "\n";
    std::cout << "back()         = " << sentence.back()   << "\n";

    // --- Concatenation ---
    std::cout << "\n--- Concatenation ---\n";
    std::string a = "Hello";
    std::string b = ", World!";
    std::string c = a + b;          // creates new string
    a += " there";                  // appends in place
    std::cout << "a: " << a << "\n";
    std::cout << "c: " << c << "\n";

    // --- Substrings ---
    std::cout << "\n--- substr ---\n";
    std::string full = "Hello, World!";
    std::cout << "substr(7, 5) = " << full.substr(7, 5) << "\n"; // "World"
    std::cout << "substr(7)    = " << full.substr(7)    << "\n"; // "World!"

    // --- Find ---
    std::cout << "\n--- find / rfind ---\n";
    std::cout << "find('World') = " << full.find("World") << "\n";  // 7
    std::cout << "find('xyz')   = " << full.find("xyz")   << "\n";  // npos
    if (full.find("World") != std::string::npos) {
        std::cout << "Found 'World'\n";
    }
    std::cout << "rfind('l')    = " << full.rfind('l')    << "\n";  // last 'l'

    // --- Replace ---
    std::cout << "\n--- replace ---\n";
    std::string r = "Hello, World!";
    r.replace(r.find("World"), 5, "C++");
    std::cout << "After replace: " << r << "\n";

    // --- Erase and insert ---
    std::cout << "\n--- erase / insert ---\n";
    std::string e = "Hello, World!";
    e.erase(5, 7);          // erase ", World"
    std::cout << "After erase(5,7): " << e << "\n";
    e.insert(5, " there");
    std::cout << "After insert:     " << e << "\n";

    // --- Compare ---
    std::cout << "\n--- compare ---\n";
    std::string x = "apple", y = "banana";
    std::cout << std::boolalpha;
    std::cout << "(x == y): " << (x == y) << "\n";
    std::cout << "(x < y):  " << (x < y)  << "  (lexicographic)\n";

    // --- Case conversion (no built-in; use transform) ---
    std::cout << "\n--- Case conversion ---\n";
    std::string mixed = "Hello World";
    std::string lower = mixed, upper = mixed;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    std::cout << "lower: " << lower << "\n";
    std::cout << "upper: " << upper << "\n";

    // --- Conversion: string <-> number ---
    std::cout << "\n--- String <-> number ---\n";
    int    num  = std::stoi("42");
    double dbl  = std::stod("3.14");
    std::string fromInt = std::to_string(num);
    std::string fromDbl = std::to_string(dbl);
    std::cout << "stoi(\"42\") = " << num  << "\n";
    std::cout << "stod(\"3.14\") = " << dbl << "\n";
    std::cout << "to_string(42) = " << fromInt << "\n";

    // --- Splitting with istringstream ---
    std::cout << "\n--- Split by whitespace ---\n";
    std::string line = "apple banana cherry date";
    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) words.push_back(word);
    for (const auto& w : words) std::cout << "[" << w << "] ";
    std::cout << "\n";

    // --- C-string interop ---
    std::cout << "\n--- c_str() ---\n";
    std::string cpp_str = "C++ string";
    const char* c_str = cpp_str.c_str();  // pointer to null-terminated char array
    std::cout << "c_str: " << c_str << "\n";

    return 0;
}
