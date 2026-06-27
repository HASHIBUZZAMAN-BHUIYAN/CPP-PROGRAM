// Day 09 — Solutions

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <iomanip>

class Student {
private:
    std::string name;
    int age;
    std::vector<double> grades;
    static int instance_count;
public:
    Student(const std::string& n, int a) : name(n), age(a) { ++instance_count; }
    ~Student() { --instance_count; }
    void add_grade(double g) { grades.push_back(g); }
    double average() const {
        if (grades.empty()) return 0;
        return std::accumulate(grades.begin(), grades.end(), 0.0) / grades.size();
    }
    void print() const {
        std::cout << "Student: " << name << " (age " << age << ")\n";
        std::cout << "Grades: ";
        for (double g : grades) std::cout << g << " ";
        std::cout << "\nAverage: " << std::fixed << std::setprecision(1) << average() << "\n";
    }
    static int get_count() { return instance_count; }
};
int Student::instance_count = 0;

class Counter {
private:
    int value, step;
public:
    Counter(int s = 1) : value(0), step(s) {}
    void increment() { value += step; }
    void decrement() { value -= step; }
    void reset()     { value = 0; }
    int  get()       { return value; }
};

class Time {
private:
    int total_seconds;
public:
    Time(int h, int m, int s) : total_seconds(h*3600 + m*60 + s) {}
    void add_seconds(int s) { total_seconds += s; }
    std::string to_string() const {
        int h = total_seconds / 3600;
        int m = (total_seconds % 3600) / 60;
        int s = total_seconds % 60;
        char buf[9];
        std::snprintf(buf, sizeof(buf), "%02d:%02d:%02d", h, m, s);
        return buf;
    }
    void print() const { std::cout << to_string() << "\n"; }
};

int main() {
    std::cout << "Exercise 1:\n";
    Student st("Alice", 20);
    st.add_grade(85); st.add_grade(92); st.add_grade(78);
    st.print();

    std::cout << "\nExercise 2:\n";
    Counter c(2);
    for (int i = 0; i < 5; ++i) c.increment();
    c.decrement();
    std::cout << "Counter: " << c.get() << "\n";
    c.reset();
    std::cout << "After reset: " << c.get() << "\n";

    std::cout << "\nExercise 3:\n";
    Time t(1, 30, 45);
    std::cout << "Start: "; t.print();
    t.add_seconds(3690);
    std::cout << "After +3690s: "; t.print();

    std::cout << "\nExercise 4:\n";
    std::cout << "Students created: " << Student::get_count() << "\n";
    {
        Student s2("Bob", 21);
        Student s3("Eve", 19);
        std::cout << "In inner scope: " << Student::get_count() << "\n";
    }
    std::cout << "After scope: " << Student::get_count() << "\n";

    return 0;
}
