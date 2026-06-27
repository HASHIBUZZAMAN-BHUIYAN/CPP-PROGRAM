// Tests for mathutils library
// Uses a minimal hand-rolled CHECK macro (same pattern as INTERMEDIATE/day10)

#include "mathutils/stats.h"
#include "mathutils/geometry.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

// ============================================================
// Minimal test harness
// ============================================================
static int g_pass = 0, g_fail = 0;

#define CHECK(cond) \
    do { \
        if (cond) { \
            ++g_pass; \
            std::cout << "  PASS: " #cond "\n"; \
        } else { \
            ++g_fail; \
            std::cout << "  FAIL: " #cond "  (" __FILE__ ":" << __LINE__ << ")\n"; \
        } \
    } while(0)

#define CHECK_NEAR(a, b, eps) \
    CHECK(std::abs((a) - (b)) < (eps))

// ============================================================
// Stats tests
// ============================================================
void test_stats() {
    std::cout << "\n[test_stats]\n";

    std::vector<double> v1 = {1, 2, 3, 4, 5};
    CHECK_NEAR(mathutils::mean(v1), 3.0, 1e-10);
    CHECK_NEAR(mathutils::median(v1), 3.0, 1e-10);
    CHECK_NEAR(mathutils::stdev(v1), std::sqrt(2.0), 1e-9);

    std::vector<double> v2 = {10, 20, 30, 40};
    CHECK_NEAR(mathutils::mean(v2), 25.0, 1e-10);
    CHECK_NEAR(mathutils::median(v2), 25.0, 1e-10);  // average of 20 and 30

    std::vector<double> v3 = {5, 5, 5, 5};
    CHECK_NEAR(mathutils::stdev(v3), 0.0, 1e-10);    // all same -> stdev = 0

    // Percentiles
    std::vector<double> p = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    CHECK_NEAR(mathutils::percentile(p, 0),   1.0, 1e-9);
    CHECK_NEAR(mathutils::percentile(p, 100), 10.0, 1e-9);
    CHECK_NEAR(mathutils::percentile(p, 50),  5.5, 1e-9);

    // Summarize
    auto s = mathutils::summarize(v1);
    CHECK_NEAR(s.min, 1.0, 1e-10);
    CHECK_NEAR(s.max, 5.0, 1e-10);
    CHECK(s.count == 5);

    // Empty data throws
    bool threw = false;
    try { mathutils::mean({}); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);
}

// ============================================================
// Geometry tests
// ============================================================
void test_geometry() {
    std::cout << "\n[test_geometry]\n";

    // Vec2
    mathutils::Vec2 a{3, 4}, b{6, 8};
    CHECK_NEAR(a.length(), 5.0, 1e-10);
    CHECK_NEAR(mathutils::distance(a, b), 5.0, 1e-10);  // same ratio, distance = 5
    CHECK_NEAR((a + b).x, 9.0, 1e-10);
    CHECK_NEAR((a + b).y, 12.0, 1e-10);
    CHECK_NEAR(a.dot(b), 50.0, 1e-10);  // 3*6 + 4*8 = 18 + 32 = 50

    auto norm = a.normalize();
    CHECK_NEAR(norm.length(), 1.0, 1e-10);
    CHECK_NEAR(norm.x, 3.0/5.0, 1e-10);

    // Circle
    mathutils::Circle c1{{0,0}, 5.0};
    CHECK_NEAR(c1.area(), 3.14159265358979 * 25.0, 1e-6);
    CHECK(c1.contains({3, 4}));         // distance = 5, on boundary
    CHECK(!c1.contains({4, 4}));        // distance = ~5.66 > 5

    mathutils::Circle c2{{8, 0}, 4.0};
    CHECK(mathutils::circles_intersect(c1, c2));  // 5+4=9 > dist=8

    mathutils::Circle c3{{20, 0}, 3.0};
    CHECK(!mathutils::circles_intersect(c1, c3)); // 5+3=8 < dist=20

    // Rect
    mathutils::Rect r{{0,0}, 10, 5};
    CHECK_NEAR(r.area(), 50.0, 1e-10);
    CHECK_NEAR(r.perimeter(), 30.0, 1e-10);
    CHECK(r.contains({5, 2}));
    CHECK(!r.contains({11, 2}));
    CHECK(!r.contains({5, 6}));
}

int main() {
    std::cout << "=== mathutils Test Suite ===\n";

    test_stats();
    test_geometry();

    std::cout << "\n--- Results ---\n";
    std::cout << "PASS: " << g_pass << "\n";
    std::cout << "FAIL: " << g_fail << "\n";
    return g_fail > 0 ? 1 : 0;
}
