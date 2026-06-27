// INTERMEDIATE Day 07 — Unit Testing with doctest
// doctest is a header-only framework: single-file include, no installation.
// Download doctest.h from: https://github.com/doctest/doctest/releases

// If doctest.h is not available, this file still compiles and runs
// with a manual stub — see the #ifdef below.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#ifdef __has_include
  #if __has_include("doctest.h")
    #include "doctest.h"
    #define HAVE_DOCTEST 1
  #else
    #define HAVE_DOCTEST 0
  #endif
#else
  #define HAVE_DOCTEST 0
#endif

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <cmath>

// ============================================================
// Code under test — a small statistics library
// ============================================================
namespace stats {

double mean(const std::vector<double>& v) {
    if (v.empty()) throw std::invalid_argument("empty vector");
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

double median(std::vector<double> v) {
    if (v.empty()) throw std::invalid_argument("empty vector");
    std::sort(v.begin(), v.end());
    size_t n = v.size();
    return (n % 2 == 0) ? (v[n/2-1] + v[n/2]) / 2.0 : v[n/2];
}

std::vector<double> mode(std::vector<double> v) {
    if (v.empty()) return {};
    std::sort(v.begin(), v.end());
    int max_count = 1, curr_count = 1;
    double curr = v[0];
    std::vector<double> result;
    for (size_t i = 1; i < v.size(); ++i) {
        if (v[i] == curr) {
            ++curr_count;
            if (curr_count > max_count) { max_count = curr_count; result = {curr}; }
            else if (curr_count == max_count) result.push_back(curr);
        } else {
            curr = v[i]; curr_count = 1;
        }
    }
    if (result.empty()) result.push_back(v[0]);
    return result;
}

double variance(const std::vector<double>& v) {
    double m = mean(v);
    double sum = 0;
    for (double x : v) sum += (x - m) * (x - m);
    return sum / v.size();
}

double stdev(const std::vector<double>& v) {
    return std::sqrt(variance(v));
}

bool is_palindrome(const std::string& s) {
    std::string copy = s;
    std::reverse(copy.begin(), copy.end());
    return s == copy;
}

} // namespace stats

// ============================================================
// Tests (only compiled when doctest.h is available)
// ============================================================
#if HAVE_DOCTEST

TEST_CASE("mean") {
    CHECK(stats::mean({1,2,3,4,5}) == doctest::Approx(3.0));
    CHECK(stats::mean({10.0}) == doctest::Approx(10.0));
    CHECK_THROWS_AS(stats::mean({}), std::invalid_argument);
}

TEST_CASE("median") {
    CHECK(stats::median({1,2,3,4,5}) == doctest::Approx(3.0));
    CHECK(stats::median({1,2,3,4}) == doctest::Approx(2.5));
    CHECK(stats::median({5}) == doctest::Approx(5.0));
    CHECK(stats::median({3,1,2}) == doctest::Approx(2.0));  // unsorted input
}

TEST_CASE("variance and stdev") {
    std::vector<double> v = {2, 4, 4, 4, 5, 5, 7, 9};
    CHECK(stats::variance(v) == doctest::Approx(4.0));
    CHECK(stats::stdev(v)    == doctest::Approx(2.0));
}

TEST_CASE("is_palindrome") {
    CHECK(stats::is_palindrome("racecar"));
    CHECK(stats::is_palindrome(""));
    CHECK(stats::is_palindrome("a"));
    CHECK_FALSE(stats::is_palindrome("hello"));
}

TEST_CASE("edge cases") {
    CHECK(stats::mean({0,0,0}) == doctest::Approx(0.0));
    CHECK(stats::median({-5,-3,-1}) == doctest::Approx(-3.0));
}

#else
// Fallback: manual test runner when doctest.h is not available
#include <iostream>
int main() {
    std::cout << "=== INTERMEDIATE Day 07: Manual Tests (doctest not found) ===\n\n";
    int pass = 0, fail = 0;
    auto check = [&](const char* name, bool ok) {
        std::cout << (ok ? "PASS" : "FAIL") << " " << name << "\n";
        ok ? ++pass : ++fail;
    };

    check("mean({1,2,3,4,5})==3",  std::abs(stats::mean({1,2,3,4,5}) - 3.0) < 1e-9);
    check("median({1,2,3,4,5})==3",std::abs(stats::median({1,2,3,4,5}) - 3.0) < 1e-9);
    check("median({1,2,3,4})==2.5",std::abs(stats::median({1,2,3,4}) - 2.5) < 1e-9);
    check("stdev correct",         std::abs(stats::stdev({2,4,4,4,5,5,7,9}) - 2.0) < 1e-9);
    check("palindrome 'racecar'",  stats::is_palindrome("racecar"));
    check("not palindrome 'hello'",!stats::is_palindrome("hello"));
    bool threw = false;
    try { stats::mean({}); } catch(...) { threw = true; }
    check("mean({}) throws",       threw);

    std::cout << "\n" << pass << " passed, " << fail << " failed.\n";
    std::cout << "\nTo use doctest:\n";
    std::cout << "1. Download doctest.h from github.com/doctest/doctest/releases\n";
    std::cout << "2. Place it next to this file\n";
    std::cout << "3. Remove the DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN define above\n";
    std::cout << "   and let doctest generate main() itself\n";
    return fail > 0 ? 1 : 0;
}
#endif
