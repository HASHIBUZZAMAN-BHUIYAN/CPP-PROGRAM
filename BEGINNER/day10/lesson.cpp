// Day 10 — OOP II: Inheritance, Virtual Functions, Polymorphism, Abstract Classes

#include <iostream>
#include <string>
#include <vector>
#include <memory>  // unique_ptr — introduced fully in Day 8 of INTERMEDIATE

// ============================================================
// Abstract base class (has at least one pure virtual function)
// ============================================================
class Shape {
protected:
    std::string color;

public:
    Shape(const std::string& c = "white") : color(c) {}
    virtual ~Shape() {}  // ALWAYS make destructors virtual in base classes

    // Pure virtual — no implementation here; derived classes MUST override
    virtual double area()      const = 0;
    virtual double perimeter() const = 0;
    virtual std::string type() const = 0;

    // Non-pure virtual — has default impl, derived classes CAN override
    virtual void describe() const {
        std::cout << type() << " [" << color << "]"
                  << "  area=" << area()
                  << "  perimeter=" << perimeter() << "\n";
    }

    std::string get_color() const { return color; }
};

// Shape cannot be instantiated directly:
// Shape s;  // error — has pure virtual methods

// ============================================================
// Derived class: Circle
// ============================================================
class Circle : public Shape {
private:
    double radius;
    static constexpr double PI = 3.14159265358979;

public:
    Circle(double r, const std::string& c = "white")
        : Shape(c), radius(r) {}

    double area()      const override { return PI * radius * radius; }
    double perimeter() const override { return 2 * PI * radius; }
    std::string type() const override { return "Circle"; }

    double get_radius() const { return radius; }
};

// ============================================================
// Derived class: Rectangle
// ============================================================
class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h, const std::string& c = "white")
        : Shape(c), width(w), height(h) {}

    double area()      const override { return width * height; }
    double perimeter() const override { return 2 * (width + height); }
    std::string type() const override { return "Rectangle"; }
};

// ============================================================
// Further derived: Square IS-A Rectangle
// ============================================================
class Square : public Rectangle {
public:
    Square(double side, const std::string& c = "white")
        : Rectangle(side, side, c) {}

    std::string type() const override { return "Square"; }
};

// ============================================================
// Demonstrates polymorphism via base-class pointer/reference
// ============================================================
void print_shape_info(const Shape& s) {
    s.describe();
}

int main() {
    std::cout << "=== Day 10: OOP II — Inheritance and Polymorphism ===\n";

    // --- Objects on the stack ---
    std::cout << "\n--- Stack objects ---\n";
    Circle    c(5.0, "red");
    Rectangle r(4.0, 6.0, "blue");
    Square    sq(3.0, "green");

    c.describe();
    r.describe();
    sq.describe();

    // --- Polymorphism via base-class reference ---
    std::cout << "\n--- Polymorphism via reference ---\n";
    print_shape_info(c);
    print_shape_info(r);
    print_shape_info(sq);

    // --- Vector of base-class pointers (classic polymorphism) ---
    std::cout << "\n--- Vector of Shape* ---\n";
    std::vector<Shape*> shapes;
    shapes.push_back(new Circle(3.0, "yellow"));
    shapes.push_back(new Rectangle(5.0, 2.0, "purple"));
    shapes.push_back(new Square(4.0, "orange"));
    shapes.push_back(new Circle(1.5));

    double total_area = 0;
    for (const Shape* s : shapes) {
        s->describe();
        total_area += s->area();
    }
    std::cout << "Total area = " << total_area << "\n";

    for (Shape* s : shapes) delete s;  // manual cleanup (next chapter: smart ptrs)
    shapes.clear();

    // --- Virtual dispatch is key ---
    std::cout << "\n--- Why 'virtual' matters ---\n";
    // Without virtual, calling a base-class pointer calls the BASE version.
    // With virtual, C++ calls the DERIVED version (correct behaviour).

    Circle* cp = new Circle(2.0);
    Shape*  sp = cp;  // base-class pointer to a Circle

    std::cout << "sp->type() = " << sp->type() << "  (correct: Circle, thanks to virtual)\n";

    delete cp;

    // --- dynamic_cast for safe downcast ---
    std::cout << "\n--- dynamic_cast ---\n";
    Shape* unknown = new Circle(4.0, "cyan");
    Circle* maybe_circle = dynamic_cast<Circle*>(unknown);
    if (maybe_circle) {
        std::cout << "dynamic_cast succeeded! radius=" << maybe_circle->get_radius() << "\n";
    }
    delete unknown;

    return 0;
}
