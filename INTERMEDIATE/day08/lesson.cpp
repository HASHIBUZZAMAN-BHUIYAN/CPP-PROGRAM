// INTERMEDIATE Day 08 — Design Patterns I: Singleton, Factory, Builder

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>

// ============================================================
// PATTERN 1: Singleton
// Ensures only ONE instance of a class exists.
// Use for: logging, configuration, resource pools, game world state.
// Warning: makes testing harder; use sparingly.
// ============================================================
class Logger {
private:
    Logger() { std::cout << "[Logger created]\n"; }
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    int log_count = 0;

public:
    static Logger& instance() {
        static Logger inst;   // guaranteed thread-safe in C++11
        return inst;
    }

    void log(const std::string& level, const std::string& msg) {
        ++log_count;
        std::cout << "[" << level << "] #" << log_count << " " << msg << "\n";
    }
    void info(const std::string& m)  { log("INFO ", m); }
    void warn(const std::string& m)  { log("WARN ", m); }
    void error(const std::string& m) { log("ERROR", m); }
    int  count() const               { return log_count; }
};

// Usage: Logger::instance().info("something happened");

// ============================================================
// PATTERN 2: Factory Method
// Creates objects without specifying the exact class.
// The creator decides what subclass to instantiate.
// Use for: plugin systems, dependency injection, testing.
// ============================================================
class Animal {
public:
    virtual ~Animal() {}
    virtual std::string speak() const = 0;
    virtual std::string name()  const = 0;
};

class Dog : public Animal {
    std::string n;
public:
    Dog(const std::string& name) : n(name) {}
    std::string speak() const override { return "Woof!"; }
    std::string name()  const override { return "Dog:" + n; }
};

class Cat : public Animal {
    std::string n;
public:
    Cat(const std::string& name) : n(name) {}
    std::string speak() const override { return "Meow!"; }
    std::string name()  const override { return "Cat:" + n; }
};

class Bird : public Animal {
    std::string n;
public:
    Bird(const std::string& name) : n(name) {}
    std::string speak() const override { return "Tweet!"; }
    std::string name()  const override { return "Bird:" + n; }
};

// The Factory — creates the right subtype based on a string
class AnimalFactory {
    using Creator = std::function<std::unique_ptr<Animal>(const std::string&)>;
    std::map<std::string, Creator> creators;

public:
    void register_type(const std::string& type, Creator creator) {
        creators[type] = creator;
    }

    std::unique_ptr<Animal> create(const std::string& type, const std::string& name) {
        auto it = creators.find(type);
        if (it == creators.end())
            throw std::invalid_argument("Unknown animal type: " + type);
        return it->second(name);
    }
};

// ============================================================
// PATTERN 3: Builder
// Constructs complex objects step by step.
// The same build process can produce different representations.
// Use for: complex objects with many optional fields (HTTP request, SQL query, GUI widget)
// ============================================================
struct Pizza {
    std::string size;
    std::string crust;
    std::vector<std::string> toppings;
    bool extra_cheese = false;
    bool gluten_free  = false;

    void print() const {
        std::cout << "Pizza: " << size << " " << crust;
        if (extra_cheese) std::cout << " +extra_cheese";
        if (gluten_free)  std::cout << " +gluten_free";
        std::cout << "\n  Toppings: ";
        for (const auto& t : toppings) std::cout << t << " ";
        std::cout << "\n";
    }
};

class PizzaBuilder {
    Pizza pizza;

public:
    PizzaBuilder& size(const std::string& s)    { pizza.size = s; return *this; }
    PizzaBuilder& crust(const std::string& c)   { pizza.crust = c; return *this; }
    PizzaBuilder& topping(const std::string& t) { pizza.toppings.push_back(t); return *this; }
    PizzaBuilder& extra_cheese()                { pizza.extra_cheese = true; return *this; }
    PizzaBuilder& gluten_free()                 { pizza.gluten_free  = true; return *this; }
    Pizza build() { return std::move(pizza); }
};

int main() {
    std::cout << "=== INTERMEDIATE Day 08: Design Patterns I ===\n";

    // --- Singleton ---
    std::cout << "\n--- Singleton: Logger ---\n";
    Logger::instance().info("Application started");
    Logger::instance().warn("Low memory");
    Logger::instance().error("Connection failed");
    std::cout << "Total log entries: " << Logger::instance().count() << "\n";

    // Both calls below access the SAME instance
    Logger& log1 = Logger::instance();
    Logger& log2 = Logger::instance();
    std::cout << "&log1 == &log2: " << std::boolalpha << (&log1 == &log2) << "\n";

    // --- Factory ---
    std::cout << "\n--- Factory: AnimalFactory ---\n";
    AnimalFactory factory;
    factory.register_type("dog",  [](const std::string& n){ return std::make_unique<Dog>(n); });
    factory.register_type("cat",  [](const std::string& n){ return std::make_unique<Cat>(n); });
    factory.register_type("bird", [](const std::string& n){ return std::make_unique<Bird>(n); });

    std::vector<std::pair<std::string,std::string>> requests = {
        {"dog","Rex"}, {"cat","Whiskers"}, {"bird","Tweety"}, {"dog","Buddy"}
    };

    for (const auto& [type, name] : requests) {
        auto animal = factory.create(type, name);
        std::cout << animal->name() << " says: " << animal->speak() << "\n";
    }

    try { factory.create("fish", "Nemo"); }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }

    // --- Builder ---
    std::cout << "\n--- Builder: PizzaBuilder ---\n";

    Pizza p1 = PizzaBuilder()
        .size("Large")
        .crust("Thin")
        .topping("Mozzarella")
        .topping("Pepperoni")
        .topping("Mushrooms")
        .extra_cheese()
        .build();
    p1.print();

    Pizza p2 = PizzaBuilder()
        .size("Small")
        .crust("Thick")
        .topping("Tomato")
        .topping("Basil")
        .gluten_free()
        .build();
    p2.print();

    return 0;
}
