// Day 10 Mini Project — Shape Drawing System
// Demonstrates polymorphism with a collection of shapes.

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <iomanip>

class Shape {
protected:
    std::string color;
public:
    Shape(const std::string& c) : color(c) {}
    virtual ~Shape() {}
    virtual double area()        const = 0;
    virtual double perimeter()   const = 0;
    virtual std::string type()   const = 0;
    virtual void draw()          const = 0;

    void print_info() const {
        std::cout << std::left << std::setw(12) << type()
                  << " color=" << std::setw(10) << color
                  << " area="  << std::setw(10) << std::fixed << std::setprecision(2) << area()
                  << " perimeter=" << perimeter() << "\n";
    }
};

class Circle : public Shape {
    double r;
    static constexpr double PI = 3.14159265358979;
public:
    Circle(double r, const std::string& c) : Shape(c), r(r) {}
    double area()      const override { return PI * r * r; }
    double perimeter() const override { return 2 * PI * r; }
    std::string type() const override { return "Circle"; }
    void draw()        const override {
        std::cout << "  Drawing " << color << " circle (r=" << r << ")\n";
    }
};

class Triangle : public Shape {
    double a, b, c;
public:
    Triangle(double a, double b, double c, const std::string& col)
        : Shape(col), a(a), b(b), c(c) {}
    double perimeter() const override { return a + b + c; }
    double area()      const override {
        double s = perimeter() / 2;
        return std::sqrt(s*(s-a)*(s-b)*(s-c));
    }
    std::string type() const override { return "Triangle"; }
    void draw()        const override {
        std::cout << "  Drawing " << color << " triangle (" << a << "," << b << "," << c << ")\n";
    }
};

class Rectangle : public Shape {
    double w, h;
public:
    Rectangle(double w, double h, const std::string& c) : Shape(c), w(w), h(h) {}
    double area()      const override { return w * h; }
    double perimeter() const override { return 2*(w+h); }
    std::string type() const override { return "Rectangle"; }
    void draw()        const override {
        std::cout << "  Drawing " << color << " rectangle (" << w << "x" << h << ")\n";
    }
};

int main() {
    std::cout << "=== Shape Drawing System ===\n\n";

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0, "red"));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0, "blue"));
    shapes.push_back(std::make_unique<Triangle>(3.0, 4.0, 5.0, "green"));
    shapes.push_back(std::make_unique<Circle>(2.5, "yellow"));
    shapes.push_back(std::make_unique<Rectangle>(10.0, 2.0, "purple"));

    std::cout << "Drawing all shapes:\n";
    for (const auto& s : shapes) s->draw();

    std::cout << "\nShape info:\n";
    double total_area = 0;
    for (const auto& s : shapes) {
        s->print_info();
        total_area += s->area();
    }
    std::cout << "\nTotal area of all shapes: " << std::fixed << std::setprecision(2) << total_area << "\n";

    // unique_ptr cleans up automatically — no delete needed
    return 0;
}
