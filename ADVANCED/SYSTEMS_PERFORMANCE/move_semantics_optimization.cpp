// ADVANCED: Move Semantics Optimization — Before/After Benchmark
//
// WHY THIS MATTERS:
// Move semantics (C++11) eliminate unnecessary data copies.
// For large objects, the difference can be orders of magnitude.
// This is why C++ can be as fast as hand-tuned C in data-intensive code.

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <numeric>
#include <algorithm>

// ============================================================
// Measurement utility
// ============================================================
struct Timer {
    std::chrono::high_resolution_clock::time_point t0;
    Timer() : t0(std::chrono::high_resolution_clock::now()) {}
    double ms() const {
        return std::chrono::duration<double, std::milli>(
            std::chrono::high_resolution_clock::now() - t0).count();
    }
};

// ============================================================
// Large object — expensive to copy, cheap to move
// ============================================================
class LargeData {
public:
    static int copy_count;
    static int move_count;
    static int construct_count;

    std::vector<int> data;
    std::string      label;

    explicit LargeData(size_t size, const std::string& l = "")
        : data(size), label(l)
    {
        std::iota(data.begin(), data.end(), 0);
        ++construct_count;
    }

    // Copy constructor — EXPENSIVE for large data
    LargeData(const LargeData& other) : data(other.data), label(other.label + "_copy") {
        ++copy_count;
    }

    // Move constructor — CHEAP (pointer swap only)
    LargeData(LargeData&& other) noexcept
        : data(std::move(other.data)), label(std::move(other.label))
    {
        ++move_count;
    }

    LargeData& operator=(const LargeData&) { ++copy_count; return *this; }
    LargeData& operator=(LargeData&&) noexcept { ++move_count; return *this; }

    size_t size() const { return data.size(); }
    long long sum() const { return std::accumulate(data.begin(), data.end(), 0LL); }
};

int LargeData::copy_count = 0;
int LargeData::move_count = 0;
int LargeData::construct_count = 0;

void reset_counts() {
    LargeData::copy_count = LargeData::move_count = LargeData::construct_count = 0;
}

// ============================================================
// BEFORE (C++03 style) — returns by copy
// ============================================================
LargeData make_data_copy_style(size_t size) {
    LargeData d(size, "copy_style");
    // Without move semantics, returning d would copy the entire vector
    return d;  // modern compiler applies NRVO — compare with explicit copy below
}

// Forces a copy (for benchmarking purposes)
LargeData force_copy(const LargeData& src) {
    return src;  // copy constructor
}

// ============================================================
// AFTER (C++11+) — returns by move
// ============================================================
LargeData make_data_move_style(size_t size) {
    LargeData d(size, "move_style");
    return d;  // NRVO or move — never copies
}

// ============================================================
// Vector push_back: copy vs emplace_back vs push_back(move)
// ============================================================
void bench_vector_operations() {
    const int N = 1000;
    const int DATA_SIZE = 10000;

    std::cout << "\n--- Vector insert benchmarks (" << N << " items, "
              << DATA_SIZE << " ints each) ---\n";

    // 1. push_back with copy
    {
        reset_counts();
        std::vector<LargeData> v;
        v.reserve(N);
        LargeData src(DATA_SIZE, "src");
        Timer t;
        for (int i = 0; i < N; ++i) v.push_back(src);  // copy each time
        std::cout << "push_back(copy):    " << t.ms() << " ms"
                  << "  copies=" << LargeData::copy_count << "\n";
    }

    // 2. push_back with move
    {
        reset_counts();
        std::vector<LargeData> v;
        v.reserve(N);
        Timer t;
        for (int i = 0; i < N; ++i) v.push_back(LargeData(DATA_SIZE));  // move from temporary
        std::cout << "push_back(move):    " << t.ms() << " ms"
                  << "  moves=" << LargeData::move_count << "\n";
    }

    // 3. emplace_back — construct in-place, zero copies OR moves for the element
    {
        reset_counts();
        std::vector<LargeData> v;
        v.reserve(N);
        Timer t;
        for (int i = 0; i < N; ++i) v.emplace_back(DATA_SIZE);  // constructs in-place
        std::cout << "emplace_back:       " << t.ms() << " ms"
                  << "  constructs=" << LargeData::construct_count << "\n";
    }
}

// ============================================================
// String operations: copy vs move
// ============================================================
void bench_string_operations() {
    const int N = 100000;
    const int STR_LEN = 1000;

    std::cout << "\n--- String benchmarks (" << N << " strings, length "
              << STR_LEN << ") ---\n";

    // Copy
    {
        std::vector<std::string> v;
        v.reserve(N);
        std::string s(STR_LEN, 'x');
        Timer t;
        for (int i = 0; i < N; ++i) v.push_back(s);  // copy
        std::cout << "string copy: " << t.ms() << " ms\n";
    }

    // Move
    {
        std::vector<std::string> v;
        v.reserve(N);
        Timer t;
        for (int i = 0; i < N; ++i) {
            std::string s(STR_LEN, 'x');
            v.push_back(std::move(s));  // move — no allocation
        }
        std::cout << "string move: " << t.ms() << " ms\n";
    }

    // emplace_back
    {
        std::vector<std::string> v;
        v.reserve(N);
        Timer t;
        for (int i = 0; i < N; ++i) v.emplace_back(STR_LEN, 'x');  // construct in-place
        std::cout << "string emplace: " << t.ms() << " ms\n";
    }
}

// ============================================================
// Swap — the original motivation for move semantics
// ============================================================
void bench_swap() {
    const int DATA_SIZE = 1000000;
    std::cout << "\n--- Swap large vectors ---\n";

    std::vector<int> a(DATA_SIZE), b(DATA_SIZE);
    std::iota(a.begin(), a.end(), 0);
    std::iota(b.begin(), b.end(), 0);

    {
        Timer t;
        std::swap(a, b);  // O(1) — just swaps internal pointers
        std::cout << "std::swap 1M ints: " << t.ms() << " ms  (pointer swap, O(1))\n";
    }
}

int main() {
    std::cout << "=== Move Semantics Optimization Benchmark ===\n";

    bench_vector_operations();
    bench_string_operations();
    bench_swap();

    std::cout << "\n=== Key Takeaways ===\n";
    std::cout << "1. Move = pointer steal (no copy), works for heap-allocated data\n";
    std::cout << "2. emplace_back is fastest: constructs in-place, no temporary\n";
    std::cout << "3. std::swap, std::sort, std::vector realloc all use moves internally\n";
    std::cout << "4. NRVO: compiler often elides even the move on function return\n";
    std::cout << "5. Small objects: copy and move cost the same (fits in registers)\n";
    std::cout << "   Large objects: move can be 10-100x faster than copy\n";

    return 0;
}
