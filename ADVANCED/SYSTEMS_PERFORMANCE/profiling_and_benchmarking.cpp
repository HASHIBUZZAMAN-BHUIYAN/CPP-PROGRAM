// ADVANCED: Profiling and Benchmarking
//
// WHY THIS MATTERS:
// "Premature optimization is the root of all evil." — Knuth
// First, MEASURE. Then, optimize only what the data says is slow.
// This file shows how to measure your code properly with std::chrono.
//
// Techniques covered:
// 1. High-resolution timing with std::chrono
// 2. Comparing different algorithms/containers on the same task
// 3. Statistical analysis of timing results (to avoid noise)
// 4. Common pitfalls in micro-benchmarking

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <string>
#include <random>
#include <iomanip>
#include <functional>
#include <cmath>

// ============================================================
// Benchmarking harness
// ============================================================
struct BenchResult {
    std::string label;
    double min_ms, max_ms, avg_ms, median_ms;
    size_t runs;

    void print() const {
        std::cout << std::left << std::setw(35) << label
                  << " avg=" << std::fixed << std::setprecision(3) << std::setw(9) << avg_ms
                  << " min=" << std::setw(9) << min_ms
                  << " median=" << std::setw(9) << median_ms
                  << " [" << runs << " runs]\n";
    }
};

// Run a function N times, collect timings, return stats
BenchResult benchmark(const std::string& label, int runs, std::function<void()> fn) {
    std::vector<double> timings;
    timings.reserve(runs);

    for (int i = 0; i < runs; ++i) {
        auto t0 = std::chrono::high_resolution_clock::now();
        fn();
        auto t1 = std::chrono::high_resolution_clock::now();
        timings.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
    }

    std::sort(timings.begin(), timings.end());
    double sum = std::accumulate(timings.begin(), timings.end(), 0.0);

    return {
        label,
        timings.front(),
        timings.back(),
        sum / runs,
        timings[runs / 2],
        static_cast<size_t>(runs)
    };
}

// ============================================================
// BENCHMARK 1: Sorting algorithm comparison
// ============================================================
void insertion_sort(std::vector<int>& v) {
    for (int i = 1; i < (int)v.size(); ++i) {
        int key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > key) { v[j+1] = v[j]; --j; }
        v[j+1] = key;
    }
}

void merge_sort(std::vector<int>& v, int lo, int hi) {
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    merge_sort(v, lo, mid);
    merge_sort(v, mid, hi);
    std::inplace_merge(v.begin()+lo, v.begin()+mid, v.begin()+hi);
}

void bench_sorting() {
    const int N = 10000, RUNS = 20;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 1000000);

    std::vector<int> original(N);
    std::generate(original.begin(), original.end(), [&]{ return dist(rng); });

    std::cout << "\n--- Sorting N=" << N << " ints, " << RUNS << " runs ---\n";

    std::vector<BenchResult> results;

    results.push_back(benchmark("std::sort", RUNS, [&]{
        auto v = original;
        std::sort(v.begin(), v.end());
    }));

    results.push_back(benchmark("std::stable_sort", RUNS, [&]{
        auto v = original;
        std::stable_sort(v.begin(), v.end());
    }));

    results.push_back(benchmark("merge_sort (handwritten)", RUNS, [&]{
        auto v = original;
        merge_sort(v, 0, v.size());
    }));

    results.push_back(benchmark("insertion_sort", RUNS, [&]{
        auto v = original;
        insertion_sort(v);
    }));

    results.push_back(benchmark("std::partial_sort (top100)", RUNS, [&]{
        auto v = original;
        std::partial_sort(v.begin(), v.begin()+100, v.end());
    }));

    for (const auto& r : results) r.print();
    std::cout << "  Note: insertion_sort is O(n^2) — much slower for large N\n";
}

// ============================================================
// BENCHMARK 2: Container lookup comparison
// ============================================================
void bench_lookup() {
    const int N = 100000, QUERIES = 50000, RUNS = 5;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, N * 2);

    // Generate N unique keys
    std::vector<int> keys(N);
    std::iota(keys.begin(), keys.end(), 1);
    // Generate QUERIES (some hits, some misses)
    std::vector<int> queries(QUERIES);
    std::generate(queries.begin(), queries.end(), [&]{ return dist(rng); });

    std::cout << "\n--- Lookup " << QUERIES << " queries in N=" << N << " items, " << RUNS << " runs ---\n";

    // std::set
    std::set<int> s(keys.begin(), keys.end());
    auto bench_set = benchmark("std::set (sorted BST, O(logN))", RUNS, [&]{
        volatile int found = 0;
        for (int q : queries) found += s.count(q);
    });
    bench_set.print();

    // std::unordered_set
    std::unordered_set<int> us(keys.begin(), keys.end());
    auto bench_us = benchmark("unordered_set (hash, O(1)avg)", RUNS, [&]{
        volatile int found = 0;
        for (int q : queries) found += us.count(q);
    });
    bench_us.print();

    // sorted vector + binary_search
    std::vector<int> sv = keys;
    auto bench_sv = benchmark("sorted vector + binary_search", RUNS, [&]{
        volatile int found = 0;
        for (int q : queries)
            found += std::binary_search(sv.begin(), sv.end(), q) ? 1 : 0;
    });
    bench_sv.print();

    // std::map lookup
    std::map<int,int> m;
    for (int k : keys) m[k] = k;
    auto bench_m = benchmark("std::map find (O(logN))", RUNS, [&]{
        volatile int found = 0;
        for (int q : queries) found += m.count(q);
    });
    bench_m.print();

    // std::unordered_map lookup
    std::unordered_map<int,int> um;
    for (int k : keys) um[k] = k;
    auto bench_um = benchmark("unordered_map find (O(1)avg)", RUNS, [&]{
        volatile int found = 0;
        for (int q : queries) found += um.count(q);
    });
    bench_um.print();
}

// ============================================================
// BENCHMARK 3: Memory access patterns
// ============================================================
void bench_memory_access() {
    const int N = 1000000, RUNS = 5;
    std::cout << "\n--- Memory access patterns N=" << N << " ---\n";

    std::vector<int> contiguous(N);
    std::iota(contiguous.begin(), contiguous.end(), 0);

    // Sequential access — cache friendly
    auto b1 = benchmark("Sequential sum (cache-friendly)", RUNS, [&]{
        volatile long long s = 0;
        for (int x : contiguous) s += x;
    });
    b1.print();

    // Strided access — cache unfriendly
    auto b2 = benchmark("Strided sum stride=16 (cache-miss)", RUNS, [&]{
        volatile long long s = 0;
        for (int i = 0; i < N; i += 16) s += contiguous[i];
    });
    b2.print();

    // Random access
    std::mt19937 rng(42);
    std::vector<int> indices(N);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), rng);

    auto b3 = benchmark("Random access (many cache misses)", RUNS, [&]{
        volatile long long s = 0;
        for (int i : indices) s += contiguous[i];
    });
    b3.print();

    std::cout << "  Cache lines are 64 bytes = 16 ints.\n";
    std::cout << "  Sequential: one cache miss per 16 accesses.\n";
    std::cout << "  Random: potentially one cache miss per access.\n";
}

int main() {
    std::cout << "=== Profiling and Benchmarking ===\n";
    std::cout << "Measuring real performance: sort, lookup, memory patterns\n";

    bench_sorting();
    bench_lookup();
    bench_memory_access();

    std::cout << "\n=== Benchmarking Best Practices ===\n";
    std::cout << "1. Run multiple times — take median (more robust than mean)\n";
    std::cout << "2. Warm up the cache: run once, discard, then measure\n";
    std::cout << "3. Use volatile to prevent the optimizer from eliding work\n";
    std::cout << "4. Compile with -O2/-O3 for realistic production performance\n";
    std::cout << "5. Profile first (gprof/perf/VTune), then optimize hot spots\n";
    std::cout << "6. Measure the whole system — micro-benchmarks can mislead\n";
    std::cout << "\nFor more serious benchmarking, use Google Benchmark library.\n";

    return 0;
}
