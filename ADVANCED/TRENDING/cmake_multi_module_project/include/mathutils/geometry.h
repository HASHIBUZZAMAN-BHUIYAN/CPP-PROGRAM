#pragma once
#include <cmath>
#include <string>

namespace mathutils {

struct Vec2 {
    double x, y;
    Vec2(double x = 0, double y = 0) : x(x), y(y) {}
    Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(double s)      const { return {x * s, y * s}; }
    double dot(const Vec2& o)     const { return x * o.x + y * o.y; }
    double length()               const { return std::sqrt(x*x + y*y); }
    Vec2   normalize()            const { double l = length(); return l > 0 ? Vec2{x/l, y/l} : Vec2{}; }
    std::string to_string() const;
};

struct Circle {
    Vec2   center;
    double radius;
    double area()         const { return 3.14159265358979 * radius * radius; }
    double circumference() const { return 2 * 3.14159265358979 * radius; }
    bool   contains(Vec2 p) const { return (p - center).length() <= radius; }
};

struct Rect {
    Vec2   origin;
    double width, height;
    double area()      const { return width * height; }
    double perimeter() const { return 2 * (width + height); }
    bool   contains(Vec2 p) const {
        return p.x >= origin.x && p.x <= origin.x + width &&
               p.y >= origin.y && p.y <= origin.y + height;
    }
};

double distance(Vec2 a, Vec2 b);
bool   circles_intersect(const Circle& a, const Circle& b);

} // namespace mathutils
