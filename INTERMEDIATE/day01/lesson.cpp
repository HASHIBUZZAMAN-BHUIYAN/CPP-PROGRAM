// INTERMEDIATE Day 01 — Templates: function templates, class templates, generic programming

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <stdexcept>

// ============================================================
// PART 1: Function Templates
// ============================================================

// A function template generates a family of functions for different types
template<typename T>
T max_of(T a, T b) {
    return a > b ? a : b;
}

// Multiple type parameters
template<typename T, typename U>
void print_pair(const T& first, const U& second) {
    std::cout << "(" << first << ", " << second << ")\n";
}

// Template with non-type parameter
template<typename T, int N>
T sum_array(const T (&arr)[N]) {
    T total = T{};  // value-initialize (0 for numerics)
    for (int i = 0; i < N; ++i) total += arr[i];
    return total;
}

// Template specialization — custom behavior for one type
template<>
std::string max_of<std::string>(std::string a, std::string b) {
    return (a.length() >= b.length()) ? a : b;  // longest string wins
}

// ============================================================
// PART 2: Class Templates
// ============================================================

// A generic pair (like std::pair but educational)
template<typename First, typename Second>
class Pair {
private:
    First  first;
    Second second;

public:
    Pair(First f, Second s) : first(f), second(s) {}

    First  get_first()  const { return first;  }
    Second get_second() const { return second; }

    void set_first(const First& f)   { first = f; }
    void set_second(const Second& s) { second = s; }

    void swap() {
        std::swap(first, second);  // only works if First == Second
    }

    friend std::ostream& operator<<(std::ostream& os, const Pair& p) {
        return os << "(" << p.first << ", " << p.second << ")";
    }
};

// Generic Stack class template
template<typename T>
class Stack {
private:
    std::vector<T> data;
    size_t max_size;

public:
    explicit Stack(size_t cap = 100) : max_size(cap) {}

    void push(const T& val) {
        if (data.size() >= max_size)
            throw std::overflow_error("Stack overflow");
        data.push_back(val);
    }

    T pop() {
        if (data.empty()) throw std::underflow_error("Stack underflow");
        T val = data.back();
        data.pop_back();
        return val;
    }

    const T& peek() const {
        if (data.empty()) throw std::underflow_error("Stack is empty");
        return data.back();
    }

    bool  empty() const { return data.empty(); }
    size_t size() const { return data.size(); }

    void print() const {
        std::cout << "Stack[";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i + 1 < data.size()) std::cout << ", ";
        }
        std::cout << "] (top=" << (data.empty() ? "empty" : std::to_string(data.size()-1)) << ")\n";
    }
};

// ============================================================
// PART 3: Template in a more realistic context
// ============================================================

// Generic min/max range function
template<typename Iterator>
auto range_minmax(Iterator begin, Iterator end)
    -> std::pair<typename std::iterator_traits<Iterator>::value_type,
                 typename std::iterator_traits<Iterator>::value_type>
{
    auto mn = *begin, mx = *begin;
    for (auto it = begin; it != end; ++it) {
        if (*it < mn) mn = *it;
        if (*it > mx) mx = *it;
    }
    return {mn, mx};
}

int main() {
    std::cout << "=== INTERMEDIATE Day 01: Templates ===\n";

    // --- Function templates ---
    std::cout << "\n--- Function templates ---\n";
    std::cout << "max_of(3, 7)       = " << max_of(3, 7)         << "\n";
    std::cout << "max_of(3.14, 2.72) = " << max_of(3.14, 2.72)   << "\n";
    std::cout << "max_of('a', 'z')   = " << max_of('a', 'z')     << "\n";
    // Specialization: picks longest string
    std::cout << "max_of strings     = " << max_of(std::string("hello"), std::string("C++")) << "\n";

    std::cout << "\n--- Multiple type params ---\n";
    print_pair("age", 30);
    print_pair(3.14, "pi");
    print_pair(true, 'X');

    std::cout << "\n--- Non-type template param ---\n";
    int   iarr[] = {1, 2, 3, 4, 5};
    double darr[] = {1.1, 2.2, 3.3};
    std::cout << "sum int[5]    = " << sum_array(iarr) << "\n";
    std::cout << "sum double[3] = " << sum_array(darr) << "\n";

    // --- Pair class template ---
    std::cout << "\n--- Pair<T,U> ---\n";
    Pair<std::string, int> person("Alice", 30);
    std::cout << "person = " << person << "\n";
    person.set_second(31);
    std::cout << "After birthday: " << person << "\n";

    Pair<double, double> coords(3.14, 2.72);
    std::cout << "coords = " << coords << "\n";

    // --- Stack<T> ---
    std::cout << "\n--- Stack<int> ---\n";
    Stack<int> si;
    si.push(1); si.push(2); si.push(3);
    si.print();
    std::cout << "pop: " << si.pop() << "\n";
    si.print();

    std::cout << "\n--- Stack<string> ---\n";
    Stack<std::string> ss;
    ss.push("hello"); ss.push("world"); ss.push("C++");
    ss.print();
    std::cout << "peek: " << ss.peek() << "\n";

    // --- Template with iterators ---
    std::cout << "\n--- range_minmax ---\n";
    std::vector<int> v = {5, 3, 9, 1, 7};
    auto [mn, mx] = range_minmax(v.begin(), v.end());
    std::cout << "min=" << mn << " max=" << mx << "\n";

    return 0;
}
