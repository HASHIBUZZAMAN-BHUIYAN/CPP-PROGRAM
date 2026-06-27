#include "mathutils/geometry.h"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace mathutils {

std::string Vec2::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << "(" << x << ", " << y << ")";
    return oss.str();
}

double distance(Vec2 a, Vec2 b) {
    return (b - a).length();
}

bool circles_intersect(const Circle& a, const Circle& b) {
    return distance(a.center, b.center) < (a.radius + b.radius);
}

} // namespace mathutils
