// Day 01 Mini Project — Personal Info Card
// Collects info from the user and prints a formatted card.

#include <iostream>
#include <string>

int main() {
    std::cout << "=== Personal Info Card Generator ===\n\n";

    std::string name, city, hobby, language;
    int age;
    double height;

    std::cout << "Enter your name: ";
    std::getline(std::cin, name);

    std::cout << "Enter your age: ";
    std::cin >> age;
    std::cin.ignore();

    std::cout << "Enter your height in meters (e.g. 1.75): ";
    std::cin >> height;
    std::cin.ignore();

    std::cout << "Enter your city: ";
    std::getline(std::cin, city);

    std::cout << "Enter your favourite hobby: ";
    std::getline(std::cin, hobby);

    std::cout << "Enter your favourite programming language: ";
    std::getline(std::cin, language);

    std::cout << "\n";
    std::cout << "==========================================\n";
    std::cout << "              PERSONAL INFO CARD          \n";
    std::cout << "==========================================\n";
    std::cout << "  Name    : " << name     << "\n";
    std::cout << "  Age     : " << age      << " years\n";
    std::cout << "  Height  : " << height   << " m\n";
    std::cout << "  City    : " << city     << "\n";
    std::cout << "  Hobby   : " << hobby    << "\n";
    std::cout << "  Language: " << language << "\n";
    std::cout << "==========================================\n";

    return 0;
}
