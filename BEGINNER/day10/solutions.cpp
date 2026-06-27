// Day 10 — Solutions

#include <iostream>
#include <string>
#include <vector>

class Animal {
protected:
    std::string name;
public:
    Animal(const std::string& n) : name(n) {}
    virtual ~Animal() {}
    virtual std::string sound() const = 0;
    virtual std::string type()  const = 0;
    virtual void describe() const {
        std::cout << type() << " '" << name << "' says: " << sound() << "\n";
    }
    const std::string& get_name() const { return name; }
};

class Dog : public Animal {
public:
    Dog(const std::string& n) : Animal(n) {}
    std::string sound() const override { return "Woof"; }
    std::string type()  const override { return "Dog";  }
};

class Cat : public Animal {
public:
    Cat(const std::string& n) : Animal(n) {}
    std::string sound() const override { return "Meow"; }
    std::string type()  const override { return "Cat";  }
};

class Bird : public Animal {
public:
    Bird(const std::string& n) : Animal(n) {}
    std::string sound() const override { return "Tweet"; }
    std::string type()  const override { return "Bird";  }
};

class Employee {
protected:
    std::string name;
    double salary;
public:
    Employee(const std::string& n, double s) : name(n), salary(s) {}
    virtual ~Employee() {}
    virtual double annual_bonus() const = 0;
    virtual std::string role()    const = 0;
    void print() const {
        std::cout << name << " [" << role() << "] salary=$" << salary
                  << " bonus=$" << annual_bonus() << "\n";
    }
};

class Manager  : public Employee {
public:
    Manager(const std::string& n, double s) : Employee(n, s) {}
    double annual_bonus() const override { return salary * 0.20; }
    std::string role()    const override { return "Manager"; }
};

class Engineer : public Employee {
public:
    Engineer(const std::string& n, double s) : Employee(n, s) {}
    double annual_bonus() const override { return salary * 0.15; }
    std::string role()    const override { return "Engineer"; }
};

class Intern   : public Employee {
public:
    Intern(const std::string& n, double s) : Employee(n, s) {}
    double annual_bonus() const override { return salary * 0.05; }
    std::string role()    const override { return "Intern"; }
};

int main() {
    std::cout << "Exercise 1:\n";
    std::vector<Animal*> animals = {
        new Dog("Rex"), new Cat("Whiskers"), new Bird("Tweety")
    };
    for (Animal* a : animals) a->describe();
    for (Animal* a : animals) delete a;

    std::cout << "\nExercise 2:\n";
    std::vector<Employee*> employees = {
        new Manager("Alice", 80000),
        new Engineer("Bob", 70000),
        new Intern("Carol", 30000)
    };
    for (Employee* e : employees) e->print();
    for (Employee* e : employees) delete e;

    std::cout << "\nExercise 3:\n";
    std::vector<Animal*> mixed = {
        new Dog("Buddy"), new Cat("Luna"), new Dog("Max"), new Bird("Pip")
    };
    for (Animal* a : mixed) {
        Dog* d = dynamic_cast<Dog*>(a);
        if (d) std::cout << "Dog found: " << d->get_name() << "\n";
    }
    for (Animal* a : mixed) delete a;

    return 0;
}
