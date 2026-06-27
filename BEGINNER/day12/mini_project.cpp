// Day 12 Mini Project — Safe CSV Reader with exception handling

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

class CsvRow {
    std::vector<std::string> fields;
public:
    CsvRow() = default;
    explicit CsvRow(const std::string& line) {
        std::istringstream ss(line);
        std::string f;
        while (std::getline(ss, f, ',')) fields.push_back(f);
    }
    const std::string& operator[](size_t i) const {
        if (i >= fields.size())
            throw std::out_of_range("Column " + std::to_string(i) + " out of range");
        return fields[i];
    }
    size_t size() const { return fields.size(); }
    void print() const {
        for (size_t i = 0; i < fields.size(); ++i) {
            std::cout << "[" << fields[i] << "]";
            if (i+1 < fields.size()) std::cout << " ";
        }
        std::cout << "\n";
    }
};

std::vector<CsvRow> read_csv(const std::string& filename) {
    std::ifstream f(filename);
    if (!f) throw std::runtime_error("Cannot open: " + filename);
    std::vector<CsvRow> rows;
    std::string line;
    while (std::getline(f, line)) {
        if (!line.empty()) rows.emplace_back(line);
    }
    return rows;
}

int main() {
    std::cout << "=== Safe CSV Reader ===\n";

    // Write a test CSV file
    {
        std::ofstream out("test.csv");
        out << "Name,Age,City\n";
        out << "Alice,30,London\n";
        out << "Bob,25,Paris\n";
        out << "Carol,35,Berlin\n";
    }

    try {
        auto rows = read_csv("test.csv");
        std::cout << "Read " << rows.size() << " rows:\n";
        for (const auto& row : rows) row.print();

        std::cout << "\nAccessing invalid column:\n";
        try {
            std::cout << rows[0][10] << "\n";
        } catch (const std::out_of_range& e) {
            std::cout << "Caught: " << e.what() << "\n";
        }

        std::cout << "\nOpening nonexistent file:\n";
        auto bad = read_csv("nope.csv");  // should throw
    } catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    return 0;
}
