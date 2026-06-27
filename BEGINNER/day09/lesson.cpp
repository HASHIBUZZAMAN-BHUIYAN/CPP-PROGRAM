// Day 09 — OOP I: Classes, Objects, Constructors, Destructors, Access Specifiers

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// ============================================================
// A well-designed class: BankAccount
// Demonstrates: private data, public interface, constructors,
// destructor, this pointer, getters/setters
// ============================================================
class BankAccount {
private:
    std::string owner;
    double balance;
    static int account_count;  // shared across all BankAccount instances

public:
    // Constructor — called when object is created
    BankAccount(const std::string& owner_name, double initial_balance = 0.0)
        : owner(owner_name), balance(initial_balance)
    {
        ++account_count;
        std::cout << "[Account created for " << owner << "]\n";
    }

    // Destructor — called when object goes out of scope
    ~BankAccount() {
        --account_count;
        std::cout << "[Account closed for " << owner << "]\n";
    }

    // Getters (const — don't modify the object)
    std::string get_owner()   const { return owner; }
    double      get_balance() const { return balance; }

    // Setter with validation
    void set_owner(const std::string& new_name) {
        if (!new_name.empty()) owner = new_name;
    }

    // Member functions
    void deposit(double amount) {
        if (amount > 0) balance += amount;
        else std::cout << "Invalid deposit amount\n";
    }

    bool withdraw(double amount) {
        if (amount <= 0) { std::cout << "Invalid amount\n"; return false; }
        if (amount > balance) { std::cout << "Insufficient funds\n"; return false; }
        balance -= amount;
        return true;
    }

    void print() const {
        std::cout << "Account[" << owner << "] balance=$" << balance << "\n";
    }

    static int get_count() { return account_count; }  // static member function
};

// Static member must be defined outside the class
int BankAccount::account_count = 0;

// ============================================================
// A simpler example: Point struct vs class
// struct = class with members public by default
// ============================================================
struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    double dist_to_origin() const {
        return std::sqrt(x*x + y*y);
    }
    void print() const {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

// ============================================================
// Class with multiple constructors (overloaded)
// ============================================================
class Rectangle {
private:
    double width, height;

public:
    Rectangle() : width(1.0), height(1.0) {}              // default constructor
    Rectangle(double w, double h) : width(w), height(h) {} // parameterized
    Rectangle(double side) : width(side), height(side) {}  // square

    double area()      const { return width * height; }
    double perimeter() const { return 2 * (width + height); }

    void print() const {
        std::cout << "Rectangle(" << width << " x " << height
                  << ") area=" << area() << "\n";
    }
};

int main() {
    std::cout << "=== Day 09: OOP I — Classes and Objects ===\n";

    // --- BankAccount ---
    std::cout << "\n--- BankAccount ---\n";
    std::cout << "Accounts before: " << BankAccount::get_count() << "\n";

    {   // inner scope — destructor fires when scope ends
        BankAccount acc1("Alice", 1000.0);
        BankAccount acc2("Bob");

        std::cout << "Accounts now: " << BankAccount::get_count() << "\n";

        acc1.deposit(500);
        acc1.print();

        acc2.deposit(200);
        if (acc2.withdraw(50)) std::cout << "Withdrawal successful\n";
        acc2.withdraw(300);  // should fail
        acc2.print();
    }   // <-- destructors called here for acc1 and acc2

    std::cout << "Accounts after scope: " << BankAccount::get_count() << "\n";

    // --- Point ---
    std::cout << "\n--- Point ---\n";
    Point p1(3.0, 4.0);
    Point p2;
    p1.print();
    p2.print();
    std::cout << "Distance p1 to origin: " << p1.dist_to_origin() << "\n";

    // --- Rectangle ---
    std::cout << "\n--- Rectangle constructors ---\n";
    Rectangle r1;            // default
    Rectangle r2(4.0, 6.0); // parameterized
    Rectangle r3(5.0);       // square

    r1.print();
    r2.print();
    r3.print();

    // --- this pointer ---
    std::cout << "\n--- this pointer ---\n";
    std::cout << "The 'this' pointer inside a method points to the object itself.\n";
    std::cout << "Used when a parameter has the same name as a member (shadowing).\n";

    return 0;
}
