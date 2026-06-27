// INTERMEDIATE Day 05 — Move Semantics: rvalue references, std::move, Rule of Five

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <utility>  // std::move

// ============================================================
// PART 1: lvalues vs rvalues
// ============================================================
// lvalue: has a persistent name/address (can take its address with &)
// rvalue: temporary, no persistent address (result of expressions, literals)
//
//   int x = 5;     x is an lvalue, 5 is an rvalue
//   foo()          return value is an rvalue (unless foo() returns a reference)

// ============================================================
// PART 2: A class demonstrating all five special functions (Rule of Five)
// ============================================================
class Buffer {
private:
    size_t size;
    int*   data;

    void log(const char* what) const {
        std::cout << "[" << what << " Buffer size=" << size << " ptr=" << (void*)data << "]\n";
    }

public:
    // Constructor
    explicit Buffer(size_t n) : size(n), data(new int[n]) {
        for (size_t i = 0; i < n; ++i) data[i] = static_cast<int>(i);
        log("Construct");
    }

    // Destructor (Rule of Five #1)
    ~Buffer() {
        log("Destruct");
        delete[] data;
    }

    // Copy constructor — DEEP copy (Rule of Five #2)
    Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
        log("Copy-Construct");
    }

    // Copy assignment — DEEP copy (Rule of Five #3)
    Buffer& operator=(const Buffer& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        data = new int[size];
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
        log("Copy-Assign");
        return *this;
    }

    // Move constructor — STEAL the resource (Rule of Five #4)
    Buffer(Buffer&& other) noexcept : size(other.size), data(other.data) {
        other.size = 0;
        other.data = nullptr;  // leave the moved-from object in a valid state
        log("Move-Construct");
    }

    // Move assignment — STEAL the resource (Rule of Five #5)
    Buffer& operator=(Buffer&& other) noexcept {
        if (this == &other) return *this;
        delete[] data;         // free our current data
        size = other.size;
        data = other.data;
        other.size = 0;
        other.data = nullptr;
        log("Move-Assign");
        return *this;
    }

    size_t get_size() const { return size; }

    // Factory function — returns by value; compiler applies NRVO/move automatically
    static Buffer create(size_t n) {
        return Buffer(n);
    }
};

// ============================================================
// PART 3: Performance benchmark — copy vs move
// ============================================================
template<typename T>
auto measure(const char* label, T&& fn) {
    auto start = std::chrono::high_resolution_clock::now();
    fn();
    auto end   = std::chrono::high_resolution_clock::now();
    double ms  = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << label << ": " << ms << " ms\n";
}

int main() {
    std::cout << "=== INTERMEDIATE Day 05: Move Semantics ===\n";

    // --- Copy vs Move demo ---
    std::cout << "\n--- Copy vs Move ---\n";
    {
        Buffer b1(5);
        Buffer b2 = b1;              // copy — allocates new memory
        Buffer b3 = std::move(b1);   // move — steals b1's pointer, no allocation
        // b1 is now empty (size=0, data=nullptr) — valid but unspecified state
        std::cout << "b2.size=" << b2.get_size() << " b3.size=" << b3.get_size() << "\n";
    }

    // --- Move assignment ---
    std::cout << "\n--- Move assignment ---\n";
    {
        Buffer b4(3), b5(10);
        b4 = std::move(b5);   // b5 is now empty
    }

    // --- Returning from a function (NRVO / RVO) ---
    std::cout << "\n--- Return value optimization ---\n";
    {
        Buffer b = Buffer::create(8);
        // Compiler often elides the copy/move entirely (NRVO)
        std::cout << "created buffer size=" << b.get_size() << "\n";
    }

    // --- std::vector and moves ---
    std::cout << "\n--- std::string move performance ---\n";
    const int N = 100000;

    // Copying strings is expensive
    measure("Copy strings to vector", [&](){
        std::vector<std::string> v;
        v.reserve(N);
        std::string s(1000, 'x');  // 1000-char string
        for (int i = 0; i < N; ++i) {
            v.push_back(s);         // copy each time
        }
    });

    // Moving strings is cheap (just pointer swap)
    measure("Move strings to vector", [&](){
        std::vector<std::string> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) {
            std::string s(1000, 'x');
            v.push_back(std::move(s));  // move — no allocation
        }
    });

    // --- Rules ---
    std::cout << "\n--- Move semantics rules ---\n";
    std::cout << "1. After std::move(x), treat x as 'valid but unspecified'\n";
    std::cout << "2. noexcept on move ops enables optimizations (vector reallocation)\n";
    std::cout << "3. Return local variables by value — compiler applies NRVO\n";
    std::cout << "4. Don't std::move the return value — prevents NRVO\n";
    std::cout << "5. Rule of Five: if you need move, you probably need all 5\n";
    std::cout << "   (But prefer RAII types to avoid writing any of them)\n";

    return 0;
}
