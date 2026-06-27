// Day 10 — Exercises: Inheritance, Virtual Functions, Polymorphism

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// Exercise 1: Animal hierarchy
// TODO: Create abstract base class Animal with:
//   - string name (protected)
//   - pure virtual: string sound() const, string type() const
//   - virtual: void describe() const (prints name, type, sound)
//   Derived classes: Dog, Cat, Bird
//   Dog.sound()="Woof", Cat.sound()="Meow", Bird.sound()="Tweet"
// YOUR CODE HERE


// Exercise 2: Employee hierarchy
// TODO: Base class Employee: name, salary, virtual double annual_bonus() const
//   - Manager: bonus = 20% of salary
//   - Engineer: bonus = 15% of salary
//   - Intern:   bonus = 5% of salary
//   All override: string role() const
// YOUR CODE HERE


int main() {
    std::cout << "=== Day 10 Exercises ===\n\n";

    // Exercise 1
    std::cout << "Exercise 1 - Animals:\n";
    // TODO: Create a vector<Animal*> with one Dog, one Cat, one Bird.
    //       Call describe() on each. Then delete all.
    // YOUR CODE HERE


    // Exercise 2
    std::cout << "\nExercise 2 - Employees:\n";
    // TODO: Create Manager("Alice", 80000), Engineer("Bob", 70000), Intern("Carol", 30000).
    //       Put them in vector<Employee*>. Print name, role, salary, bonus for each.
    // YOUR CODE HERE


    // Exercise 3: dynamic_cast practice
    // TODO: Create a vector<Animal*> with mixed types.
    //       Use dynamic_cast<Dog*> to find all Dogs and print "Dog found: <name>".
    std::cout << "\nExercise 3 - dynamic_cast:\n";
    // YOUR CODE HERE


    return 0;
}
