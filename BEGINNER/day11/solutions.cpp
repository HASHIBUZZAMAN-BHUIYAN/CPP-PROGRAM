// Day 11 — Solutions

#include <iostream>
#include <cstring>
#include <cmath>

struct Matrix2x2 {
    double d[2][2];
    Matrix2x2(double a,double b,double c,double dd) { d[0][0]=a;d[0][1]=b;d[1][0]=c;d[1][1]=dd; }
    Matrix2x2 operator+(const Matrix2x2& r) const {
        return {d[0][0]+r.d[0][0], d[0][1]+r.d[0][1], d[1][0]+r.d[1][0], d[1][1]+r.d[1][1]};
    }
    Matrix2x2 operator*(const Matrix2x2& r) const {
        return {d[0][0]*r.d[0][0]+d[0][1]*r.d[1][0],
                d[0][0]*r.d[0][1]+d[0][1]*r.d[1][1],
                d[1][0]*r.d[0][0]+d[1][1]*r.d[1][0],
                d[1][0]*r.d[0][1]+d[1][1]*r.d[1][1]};
    }
    double determinant() const { return d[0][0]*d[1][1] - d[0][1]*d[1][0]; }
    friend std::ostream& operator<<(std::ostream& os, const Matrix2x2& m) {
        return os << "[" << m.d[0][0] << " " << m.d[0][1] << "; "
                         << m.d[1][0] << " " << m.d[1][1] << "]";
    }
};

class MyString {
    char* data;
public:
    MyString(const char* s = "") {
        data = new char[strlen(s)+1];
        strcpy(data, s);
    }
    ~MyString() { delete[] data; }
    MyString(const MyString& o) {
        data = new char[strlen(o.data)+1];
        strcpy(data, o.data);
    }
    MyString& operator=(const MyString& o) {
        if (this == &o) return *this;
        delete[] data;
        data = new char[strlen(o.data)+1];
        strcpy(data, o.data);
        return *this;
    }
    MyString& operator+=(const MyString& o) {
        char* tmp = new char[strlen(data)+strlen(o.data)+1];
        strcpy(tmp, data);
        strcat(tmp, o.data);
        delete[] data;
        data = tmp;
        return *this;
    }
    bool operator==(const MyString& o) const { return strcmp(data, o.data) == 0; }
    friend std::ostream& operator<<(std::ostream& os, const MyString& s) {
        return os << s.data;
    }
};

struct Complex {
    double real, imag;
    Complex(double r=0, double i=0): real(r), imag(i) {}
    Complex operator+(const Complex& o) const { return {real+o.real, imag+o.imag}; }
    Complex operator-(const Complex& o) const { return {real-o.real, imag-o.imag}; }
    Complex operator*(const Complex& o) const {
        return {real*o.real - imag*o.imag, real*o.imag + imag*o.real};
    }
    bool operator==(const Complex& o) const { return real==o.real && imag==o.imag; }
    double magnitude() const { return std::sqrt(real*real + imag*imag); }
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        return os << c.real << (c.imag >= 0 ? "+" : "") << c.imag << "i";
    }
};

int main() {
    std::cout << "Exercise 1:\n";
    Matrix2x2 A(1,2,3,4), B(5,6,7,8);
    std::cout << "A = " << A << "\n";
    std::cout << "B = " << B << "\n";
    std::cout << "A+B = " << (A+B) << "\n";
    std::cout << "A*B = " << (A*B) << "\n";
    std::cout << "det(A) = " << A.determinant() << "\n";

    std::cout << "\nExercise 2:\n";
    MyString s1("hello"), s2(s1);
    MyString s3("world");
    s3 = s1;
    s1 += MyString(" world");
    std::cout << "s1=" << s1 << " s2=" << s2 << " s3=" << s3 << "\n";
    std::cout << "s2==s3? " << std::boolalpha << (s2 == s3) << "\n";

    std::cout << "\nExercise 3:\n";
    Complex c1(3,4), c2(1,2);
    std::cout << "c1=" << c1 << " c2=" << c2 << "\n";
    std::cout << "c1+c2=" << (c1+c2) << "\n";
    std::cout << "c1*c2=" << (c1*c2) << "\n";
    std::cout << "|c1|=" << c1.magnitude() << "\n";

    return 0;
}
