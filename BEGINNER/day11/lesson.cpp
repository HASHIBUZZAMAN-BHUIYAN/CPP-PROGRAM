// Day 11 — Operator Overloading & Rule of Three/Five

#include <iostream>
#include <string>
#include <cstring>  // strcpy, strlen
#include <cmath>

// ============================================================
// Vector2D — demonstrates operator overloading cleanly
// ============================================================
class Vector2D {
public:
    double x, y;

    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    // Arithmetic operators (return new objects — don't modify this)
    Vector2D operator+(const Vector2D& rhs) const {
        return Vector2D(x + rhs.x, y + rhs.y);
    }
    Vector2D operator-(const Vector2D& rhs) const {
        return Vector2D(x - rhs.x, y - rhs.y);
    }
    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    // Compound assignment (modify this, return reference)
    Vector2D& operator+=(const Vector2D& rhs) {
        x += rhs.x; y += rhs.y; return *this;
    }

    // Comparison
    bool operator==(const Vector2D& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Vector2D& rhs) const {
        return !(*this == rhs);
    }

    // Unary negation
    Vector2D operator-() const { return Vector2D(-x, -y); }

    double magnitude() const { return std::sqrt(x*x + y*y); }

    // Stream output as a friend (non-member, but needs private access)
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};

// Non-member scalar * vector (so we can write 3.0 * v as well as v * 3.0)
Vector2D operator*(double scalar, const Vector2D& v) {
    return v * scalar;
}

// ============================================================
// RawString — demonstrates the Rule of Three
// (copy constructor, copy assignment, destructor)
// ONLY needed when you manage raw resources — smart ptrs avoid this!
// ============================================================
class RawString {
private:
    char* data;
    size_t len;

public:
    // Constructor
    RawString(const char* s = "") {
        len = std::strlen(s);
        data = new char[len + 1];
        std::strcpy(data, s);
        std::cout << "[Construct \"" << data << "\"]\n";
    }

    // Destructor (Rule of Three #1)
    ~RawString() {
        std::cout << "[Destruct \"" << data << "\"]\n";
        delete[] data;
    }

    // Copy constructor (Rule of Three #2)
    RawString(const RawString& other) {
        len = other.len;
        data = new char[len + 1];
        std::strcpy(data, other.data);
        std::cout << "[Copy \"" << data << "\"]\n";
    }

    // Copy assignment (Rule of Three #3)
    RawString& operator=(const RawString& other) {
        if (this == &other) return *this;  // self-assignment guard
        delete[] data;                      // free old resource
        len = other.len;
        data = new char[len + 1];
        std::strcpy(data, other.data);
        std::cout << "[Assign \"" << data << "\"]\n";
        return *this;
    }

    // Subscript operator
    char& operator[](size_t i)       { return data[i]; }
    char  operator[](size_t i) const { return data[i]; }

    friend std::ostream& operator<<(std::ostream& os, const RawString& rs) {
        return os << rs.data;
    }

    size_t length() const { return len; }
};

// ============================================================
// Fraction — stream input operator
// ============================================================
class Fraction {
public:
    int num, den;
    Fraction(int n = 0, int d = 1) : num(n), den(d) {}

    Fraction operator+(const Fraction& r) const {
        return Fraction(num*r.den + r.num*den, den*r.den);
    }
    bool operator<(const Fraction& r) const {
        return num * r.den < r.num * den;
    }
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        return os << f.num << "/" << f.den;
    }
    friend std::istream& operator>>(std::istream& is, Fraction& f) {
        char slash;
        is >> f.num >> slash >> f.den;
        return is;
    }
};

int main() {
    std::cout << "=== Day 11: Operator Overloading & Rule of Three ===\n";

    // --- Vector2D ---
    std::cout << "\n--- Vector2D ---\n";
    Vector2D a(1, 2), b(3, 4);
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "a + b = " << (a + b) << "\n";
    std::cout << "a - b = " << (a - b) << "\n";
    std::cout << "a * 3 = " << (a * 3) << "\n";
    std::cout << "3 * a = " << (3 * a) << "\n";
    std::cout << "-a    = " << (-a)    << "\n";
    std::cout << "|b|   = " << b.magnitude() << "\n";
    std::cout << "a==b? " << std::boolalpha << (a == b) << "\n";
    a += b;
    std::cout << "a += b: a = " << a << "\n";

    // --- Rule of Three ---
    std::cout << "\n--- Rule of Three (RawString) ---\n";
    {
        RawString s1("hello");
        RawString s2 = s1;        // copy constructor
        RawString s3("world");
        s3 = s1;                   // copy assignment
        std::cout << "s1=" << s1 << " s2=" << s2 << " s3=" << s3 << "\n";
        s1[0] = 'H';              // subscript operator
        std::cout << "After s1[0]='H': " << s1 << "\n";
    }   // <-- destructors fire here

    // --- Fraction ---
    std::cout << "\n--- Fraction ---\n";
    Fraction f1(1, 2), f2(1, 3);
    std::cout << f1 << " + " << f2 << " = " << (f1 + f2) << "\n";
    std::cout << f1 << " < " << f2 << "? " << (f1 < f2) << "\n";
    std::cout << "Enter a fraction (e.g. 3/4): ";
    Fraction f3;
    std::cin >> f3;
    std::cout << "You entered: " << f3 << "\n";

    return 0;
}
