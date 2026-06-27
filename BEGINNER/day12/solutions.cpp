// Day 12 — Solutions

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <climits>

class AppException : public std::runtime_error {
public:
    AppException(const std::string& m) : std::runtime_error(m) {}
};
class NetworkException : public AppException {
public:
    NetworkException(const std::string& m) : AppException("Network: " + m) {}
};
class TimeoutException : public NetworkException {
public:
    TimeoutException() : NetworkException("Connection timed out") {}
};

template<typename T>
const T& safe_get(const std::vector<T>& v, int idx) {
    if (idx < 0 || idx >= static_cast<int>(v.size()))
        throw std::out_of_range("Index " + std::to_string(idx)
            + " out of range [0," + std::to_string(v.size()) + ")");
    return v[idx];
}

int parse_int(const std::string& s) {
    for (char c : s) if (!std::isdigit(c) && !(c=='-' && &c==&s[0]))
        throw std::invalid_argument("Not a number: " + s);
    try {
        size_t pos;
        int val = std::stoi(s, &pos);
        if (pos != s.size()) throw std::invalid_argument("Not a number: " + s);
        return val;
    } catch (const std::out_of_range&) {
        throw std::out_of_range("Value too large: " + s);
    }
}

class SafeFile {
    std::ifstream f;
public:
    SafeFile(const std::string& name) {
        f.open(name);
        if (!f) throw std::runtime_error("Cannot open: " + name);
    }
    ~SafeFile() { if (f.is_open()) f.close(); }
    bool read_line(std::string& line) { return static_cast<bool>(std::getline(f, line)); }
};

int safe_abs(int n) noexcept { return n < 0 ? -n : n; }

int main() {
    std::cout << "Exercise 1:\n";
    try { throw TimeoutException(); }
    catch (const TimeoutException& e) { std::cout << "TimeoutException: " << e.what() << "\n"; }
    try { throw TimeoutException(); }
    catch (const AppException& e) { std::cout << "AppException: " << e.what() << "\n"; }

    std::cout << "\nExercise 2:\n";
    std::vector<int> v = {10,20,30};
    for (int i : {0, 2, 5, -1}) {
        try { std::cout << "v[" << i << "]=" << safe_get(v, i) << "\n"; }
        catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }
    }

    std::cout << "\nExercise 3:\n";
    for (const std::string& s : {"42", "abc", "99999999999"}) {
        try { std::cout << parse_int(s) << "\n"; }
        catch (const std::exception& e) { std::cout << e.what() << "\n"; }
    }

    std::cout << "\nExercise 4:\n";
    try {
        SafeFile sf("no_such_file.txt");
    } catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    std::cout << "\nExercise 5:\n";
    std::cout << "safe_abs(-42) = " << safe_abs(-42) << "\n";
    std::cout << "safe_abs(7)   = " << safe_abs(7)   << "\n";

    return 0;
}
