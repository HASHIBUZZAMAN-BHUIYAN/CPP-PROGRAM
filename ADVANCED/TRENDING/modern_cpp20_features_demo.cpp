// ADVANCED TRENDING: C++20 Features Demo
//
// g++ 16.1.0 has full C++20 support. Compile with: -std=c++20
//
// C++20 FEATURES COVERED:
// 1. Concepts  — constrain template type parameters
// 2. Ranges    — composable lazy algorithms (views::filter, views::transform)
// 3. std::span — non-owning view over contiguous data (safer than raw pointers)
// 4. Coroutines — cooperative multitasking (generator pattern)
// 5. Designated initializers — {.x = 1, .y = 2}
// 6. Three-way comparison (spaceship operator <=>)
// 7. consteval / constinit
// 8. std::format (C++20 — g++ 13+ with libstdc++ supports it)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <concepts>
#include <ranges>
#include <span>
#include <compare>
#include <array>
#include <cmath>
#include <type_traits>

// std::format (C++20, available in g++ 13+ with libstdc++)
// Try to include — graceful fallback if not available
#if __has_include(<format>)
  #include <format>
  #define HAS_FORMAT 1
#else
  #define HAS_FORMAT 0
#endif

// ============================================================
// 1. CONCEPTS — constrain template parameters at compile time
// ============================================================
// Before C++20: SFINAE or static_assert (ugly)
// C++20: requires clauses and concept definitions (clean)

// Define a concept: type must support +, -, *, /
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

// Concept using requires expression
template<typename T>
concept Printable = requires(T t) {
    { std::cout << t } -> std::same_as<std::ostream&>;
};

// Concept composition
template<typename T>
concept NumericPrintable = Numeric<T> && Printable<T>;

// Function constrained by concept
template<Numeric T>
T square(T x) { return x * x; }

template<Numeric T>
T average(const std::vector<T>& v) {
    if (v.empty()) return T{};
    return std::accumulate(v.begin(), v.end(), T{}) / static_cast<T>(v.size());
}

// Concept for containers
template<typename C>
concept Container = requires(C c) {
    c.begin(); c.end(); c.size();
    typename C::value_type;
};

template<Container C>
void print_all(const C& container, const std::string& label) {
    std::cout << label << ": [";
    bool first = true;
    for (const auto& x : container) {
        if (!first) std::cout << ", ";
        std::cout << x;
        first = false;
    }
    std::cout << "]\n";
}

void demo_concepts() {
    std::cout << "\n=== 1. Concepts ===\n";

    // These compile (satisfy Numeric)
    std::cout << "square(7)   = " << square(7)    << "\n";
    std::cout << "square(3.5) = " << square(3.5)  << "\n";

    std::vector<int> nums = {10, 20, 30, 40, 50};
    std::cout << "average({10,20,30,40,50}) = " << average(nums) << "\n";

    print_all(nums, "ints");
    print_all(std::vector<double>{1.1, 2.2, 3.3}, "doubles");

    // Compile-time check
    static_assert(Numeric<int>);
    static_assert(Numeric<double>);
    static_assert(!Numeric<std::string>);
    std::cout << "Concept checks (static_assert): all passed\n";
    std::cout << "  Numeric<int>     = true\n";
    std::cout << "  Numeric<double>  = true\n";
    std::cout << "  Numeric<string>  = false (enforced at compile time)\n";
}

// ============================================================
// 2. RANGES — composable lazy transformations
// ============================================================
void demo_ranges() {
    std::cout << "\n=== 2. Ranges (C++20) ===\n";

    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Chain: filter even numbers, then square them, then take first 3
    auto result = nums
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(3);

    std::cout << "Evens squared (first 3): [";
    bool first = true;
    for (int x : result) {
        if (!first) std::cout << ", ";
        std::cout << x;
        first = false;
    }
    std::cout << "]\n";  // Expected: [4, 16, 36]

    // iota: generate range 1..20, keep multiples of 3
    auto multiples_of_3 = std::views::iota(1, 21)
        | std::views::filter([](int x){ return x % 3 == 0; });

    std::cout << "Multiples of 3 in [1,20]: [";
    first = true;
    for (int x : multiples_of_3) {
        if (!first) std::cout << ", ";
        std::cout << x;
        first = false;
    }
    std::cout << "]\n";

    // Ranges reverse
    auto reversed = nums | std::views::reverse | std::views::take(4);
    std::cout << "Last 4 reversed: [";
    first = true;
    for (int x : reversed) {
        if (!first) std::cout << ", ";
        std::cout << x;
        first = false;
    }
    std::cout << "]\n";

    // Collect range into vector
    auto squares = std::views::iota(1, 6)
                 | std::views::transform([](int x){ return x*x; });
    std::vector<int> sq_vec(squares.begin(), squares.end());
    print_all(sq_vec, "Squares [1..5]");

    std::cout << "Key insight: ranges are LAZY — no computation until iterated\n";
}

// ============================================================
// 3. std::span — safe non-owning view of contiguous data
// ============================================================
// Before C++20: void foo(int* p, size_t n)  — raw pointer, unsafe
// C++20: void foo(std::span<int> s)          — bounds-aware, clear

double sum_span(std::span<const double> data) {
    double s = 0;
    for (double x : data) s += x;
    return s;
}

void process_middle(std::span<int> data) {
    // Get a sub-view without copying
    auto middle = data.subspan(1, data.size() - 2);
    for (int& x : middle) x *= 2;
}

void demo_span() {
    std::cout << "\n=== 3. std::span ===\n";

    // Works with vector, array, C-array — no copies
    std::vector<double> vec = {1.0, 2.5, 3.7, 4.2, 5.8};
    std::array<double, 3>  arr = {10.0, 20.0, 30.0};
    double c_arr[] = {100.0, 200.0, 300.0};

    std::cout << "sum_span(vector): " << sum_span(vec) << "\n";
    std::cout << "sum_span(array):  " << sum_span(arr) << "\n";
    std::cout << "sum_span(c_arr):  " << sum_span(c_arr) << "\n";

    // Subspan — zero-copy slice
    std::vector<int> v = {10, 20, 30, 40, 50};
    process_middle(v);
    print_all(v, "After process_middle (elements 1-3 doubled)");

    std::cout << "span.size()=" << std::span(vec).size()
              << "  span.data()=ptr to first element\n";
    std::cout << "No allocation, no copy — span is just (ptr, size)\n";
}

// ============================================================
// 4. THREE-WAY COMPARISON (spaceship operator <=>)
// ============================================================
struct Version {
    int major, minor, patch;

    // Single <=> replaces all 6 comparison operators
    auto operator<=>(const Version&) const = default;

    friend std::ostream& operator<<(std::ostream& os, const Version& v) {
        return os << v.major << "." << v.minor << "." << v.patch;
    }
};

void demo_spaceship() {
    std::cout << "\n=== 4. Three-Way Comparison (Spaceship <=>) ===\n";

    Version v1{1, 2, 3}, v2{1, 3, 0}, v3{1, 2, 3};

    std::cout << v1 << " < " << v2 << " : " << (v1 < v2 ? "true" : "false") << "\n";
    std::cout << v2 << " > " << v1 << " : " << (v2 > v1 ? "true" : "false") << "\n";
    std::cout << v1 << " == " << v3 << " : " << (v1 == v3 ? "true" : "false") << "\n";

    std::vector<Version> versions = {{2,0,0}, {1,9,5}, {1,2,3}, {2,1,0}};
    std::sort(versions.begin(), versions.end());
    std::cout << "Sorted versions: ";
    for (const auto& v : versions) std::cout << v << "  ";
    std::cout << "\n";

    std::cout << "operator<=> = default generates all 6 comparisons automatically\n";
}

// ============================================================
// 5. DESIGNATED INITIALIZERS
// ============================================================
struct Config {
    std::string host  = "localhost";
    int         port  = 8080;
    bool        tls   = false;
    int         timeout_ms = 5000;
};

void demo_designated_init() {
    std::cout << "\n=== 5. Designated Initializers ===\n";

    // Only specify the fields you care about; rest keep defaults
    Config default_cfg{};
    Config custom_cfg{
        .host = "example.com",
        .port = 443,
        .tls  = true
        // timeout_ms stays at 5000
    };

    auto print_cfg = [](const Config& c, const std::string& name) {
        std::cout << name << ": host=" << c.host << " port=" << c.port
                  << " tls=" << c.tls << " timeout=" << c.timeout_ms << "ms\n";
    };

    print_cfg(default_cfg, "default");
    print_cfg(custom_cfg,  "custom ");
}

// ============================================================
// 6. consteval — must be evaluated at compile time
// ============================================================
consteval int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

void demo_consteval() {
    std::cout << "\n=== 6. consteval (compile-time only) ===\n";

    constexpr int f5  = factorial(5);   // computed at compile time
    constexpr int f10 = factorial(10);
    std::cout << "factorial(5)  = " << f5  << "  (computed at compile time)\n";
    std::cout << "factorial(10) = " << f10 << "  (computed at compile time)\n";
    std::cout << "Zero runtime cost — the literal 120 is baked into the binary\n";
}

// ============================================================
// 7. std::format (C++20, if available)
// ============================================================
void demo_format() {
    std::cout << "\n=== 7. std::format ===\n";
#if HAS_FORMAT
    std::string s = std::format("Hello, {}! You are {} years old.", "Alice", 30);
    std::cout << s << "\n";
    std::cout << std::format("Pi = {:.4f}\n", 3.14159265);
    std::cout << std::format("{:>10} | {:>10} | {:>10}\n", "Name", "Score", "Grade");
    std::cout << std::format("{:>10} | {:>10} | {:>10}\n", "Alice", 95, "A");
    std::cout << std::format("{:>10} | {:>10} | {:>10}\n", "Bob",   82, "B");
    std::cout << "std::format available on this compiler.\n";
#else
    std::cout << "std::format not available on this build.\n";
    std::cout << "Use snprintf or libfmt as alternatives.\n";
#endif
}

int main() {
    std::cout << "=== C++20 Features Demo ===\n";
    std::cout << "Compiled with -std=c++20, g++ " __VERSION__ "\n";

    demo_concepts();
    demo_ranges();
    demo_span();
    demo_spaceship();
    demo_designated_init();
    demo_consteval();
    demo_format();

    std::cout << "\n=== C++20 Summary ===\n";
    std::cout << "Concepts:              cleaner template constraints\n";
    std::cout << "Ranges:                composable lazy algorithms (| pipe syntax)\n";
    std::cout << "std::span:             safe non-owning views of arrays\n";
    std::cout << "Spaceship <=>:         single operator generates all comparisons\n";
    std::cout << "Designated init:       partial struct initialization\n";
    std::cout << "consteval:             guaranteed compile-time evaluation\n";
    std::cout << "std::format:           Python-style string formatting\n";
    std::cout << "Coroutines:            co_await/co_yield (not shown; needs more setup)\n";
    std::cout << "Modules:               replacement for headers (not shown; limited IDE support)\n";

    return 0;
}
