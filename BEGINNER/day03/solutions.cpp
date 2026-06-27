// Day 03 — Solutions

#include <iostream>
#include <string>

int main() {
    std::cout << "=== Day 03 Solutions ===\n\n";

    // Exercise 1: BMI
    std::cout << "Exercise 1:\n";
    std::cout << "Weight (kg): ";
    double weight; std::cin >> weight;
    std::cout << "Height (m): ";
    double height; std::cin >> height;
    double bmi = weight / (height * height);
    std::cout << "BMI = " << bmi << " -> ";
    if      (bmi < 18.5) std::cout << "Underweight\n";
    else if (bmi < 25.0) std::cout << "Normal\n";
    else if (bmi < 30.0) std::cout << "Overweight\n";
    else                 std::cout << "Obese\n";

    // Exercise 2: Login
    std::cout << "\nExercise 2:\n";
    const std::string CORRECT_USER = "admin";
    const std::string CORRECT_PASS = "cpp2024";
    std::string user, pass;
    std::cout << "Username: "; std::cin >> user;
    std::cout << "Password: "; std::cin >> pass;
    if (user == CORRECT_USER && pass == CORRECT_PASS)
        std::cout << "Access granted.\n";
    else
        std::cout << "Access denied.\n";

    // Exercise 3: Season
    std::cout << "\nExercise 3:\n";
    std::cout << "Month (1-12): ";
    int month; std::cin >> month;
    switch (month) {
        case 12: case 1: case 2:  std::cout << "Winter\n"; break;
        case 3:  case 4: case 5:  std::cout << "Spring\n"; break;
        case 6:  case 7: case 8:  std::cout << "Summer\n"; break;
        case 9:  case 10: case 11: std::cout << "Autumn\n"; break;
        default: std::cout << "Invalid month\n"; break;
    }

    // Exercise 4: Largest of three
    std::cout << "\nExercise 4:\n";
    std::cout << "Enter three integers: ";
    int a, b, c; std::cin >> a >> b >> c;
    int largest = a;
    if (b > largest) largest = b;
    if (c > largest) largest = c;
    std::cout << "Largest = " << largest << "\n";

    // Exercise 5: Calculator
    std::cout << "\nExercise 5:\n";
    std::cout << "Enter: num1 op num2 (e.g. 5 + 3): ";
    double n1, n2; char op;
    std::cin >> n1 >> op >> n2;
    switch (op) {
        case '+': std::cout << n1 << " + " << n2 << " = " << n1 + n2 << "\n"; break;
        case '-': std::cout << n1 << " - " << n2 << " = " << n1 - n2 << "\n"; break;
        case '*': std::cout << n1 << " * " << n2 << " = " << n1 * n2 << "\n"; break;
        case '/':
            if (n2 == 0) std::cout << "Division by zero!\n";
            else std::cout << n1 << " / " << n2 << " = " << n1 / n2 << "\n";
            break;
        default: std::cout << "Unknown operator.\n"; break;
    }

    return 0;
}
