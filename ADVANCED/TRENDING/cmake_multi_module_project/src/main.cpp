// cmake_multi_module_project: main application
// Links against the mathutils static library (stats + geometry modules)

#include "mathutils/stats.h"
#include "mathutils/geometry.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

int main() {
    std::cout << "=== CMake Multi-Module Project Demo ===\n";
    std::cout << "Library: mathutils (stats module + geometry module)\n\n";

    // ---- Stats module ----
    std::cout << "--- Statistics ---\n";

    std::mt19937 rng(42);
    std::normal_distribution<double> dist(50.0, 15.0);
    std::vector<double> data;
    for (int i = 0; i < 1000; ++i) data.push_back(dist(rng));

    mathutils::SummaryStats s = mathutils::summarize(data);
    std::cout << "Normal(50, 15), n=1000:\n  " << s.to_string() << "\n";

    std::cout << "  p25=" << mathutils::percentile(data, 25)
              << " p75=" << mathutils::percentile(data, 75)
              << " p99=" << mathutils::percentile(data, 99) << "\n";

    // Small dataset
    std::vector<double> grades = {72, 85, 91, 68, 79, 95, 88, 63, 77, 84};
    std::cout << "\nStudent grades: ";
    for (double g : grades) std::cout << g << " ";
    std::cout << "\n  " << mathutils::summarize(grades).to_string() << "\n";

    // ---- Geometry module ----
    std::cout << "\n--- Geometry ---\n";

    mathutils::Vec2 a{3, 4}, b{6, 8};
    std::cout << "a=" << a.to_string() << " b=" << b.to_string() << "\n";
    std::cout << "distance(a, b) = " << mathutils::distance(a, b) << "\n";
    std::cout << "a + b = " << (a + b).to_string() << "\n";
    std::cout << "a . b = " << a.dot(b) << "\n";
    std::cout << "|a| = " << a.length() << "\n";
    std::cout << "a.normalize() = " << a.normalize().to_string() << "\n";

    mathutils::Circle c1{{0, 0}, 5.0};
    mathutils::Circle c2{{8, 0}, 4.0};
    mathutils::Circle c3{{20, 0}, 3.0};
    std::cout << "\nCircle c1: center=(0,0) r=5 area=" << c1.area() << "\n";
    std::cout << "c1 intersects c2 (dist=8, r1+r2=9): "
              << (mathutils::circles_intersect(c1, c2) ? "yes" : "no") << "\n";
    std::cout << "c1 intersects c3 (dist=20, r1+r2=8): "
              << (mathutils::circles_intersect(c1, c3) ? "yes" : "no") << "\n";

    mathutils::Rect rect{{1, 1}, 10, 6};
    std::cout << "\nRect: origin=(1,1) 10x6  area=" << rect.area() << "\n";
    std::cout << "contains (5,3): " << (rect.contains({5,3}) ? "yes" : "no") << "\n";
    std::cout << "contains (15,3): " << (rect.contains({15,3}) ? "yes" : "no") << "\n";

    std::cout << "\n=== Build Structure ===\n";
    std::cout << "cmake_multi_module_project/\n";
    std::cout << "├── CMakeLists.txt         -- root: defines mathutils lib + math_app + tests\n";
    std::cout << "├── include/mathutils/\n";
    std::cout << "│   ├── stats.h\n";
    std::cout << "│   └── geometry.h\n";
    std::cout << "├── src/\n";
    std::cout << "│   ├── stats.cpp\n";
    std::cout << "│   ├── geometry.cpp\n";
    std::cout << "│   └── main.cpp\n";
    std::cout << "└── tests/\n";
    std::cout << "    └── test_mathutils.cpp\n";

    return 0;
}
