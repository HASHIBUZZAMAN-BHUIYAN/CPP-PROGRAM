// Day 09 Mini Project — Library Book Catalog
// A simple catalog demonstrating class design with OOP I concepts.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

class Book {
private:
    std::string title, author, isbn;
    int year;
    bool available;

public:
    Book(const std::string& t, const std::string& a, const std::string& i, int y)
        : title(t), author(a), isbn(i), year(y), available(true) {}

    bool checkout() {
        if (!available) return false;
        available = false;
        return true;
    }

    bool return_book() {
        if (available) return false;
        available = true;
        return true;
    }

    const std::string& get_title()  const { return title;  }
    const std::string& get_author() const { return author; }
    const std::string& get_isbn()   const { return isbn;   }
    bool  is_available()            const { return available; }

    void print() const {
        std::cout << std::left
                  << std::setw(30) << title
                  << std::setw(20) << author
                  << std::setw(15) << isbn
                  << year << "  "
                  << (available ? "Available" : "Checked out")
                  << "\n";
    }
};

class Library {
private:
    std::vector<Book> books;

public:
    void add_book(const Book& b) { books.push_back(b); }

    void list_all() const {
        std::cout << std::left
                  << std::setw(30) << "Title"
                  << std::setw(20) << "Author"
                  << std::setw(15) << "ISBN"
                  << "Year  Status\n";
        std::cout << std::string(90, '-') << "\n";
        for (const auto& b : books) b.print();
    }

    Book* find_by_title(const std::string& t) {
        for (auto& b : books)
            if (b.get_title() == t) return &b;
        return nullptr;
    }

    void checkout(const std::string& title) {
        Book* b = find_by_title(title);
        if (!b) { std::cout << "Book not found.\n"; return; }
        if (b->checkout()) std::cout << "Checked out: " << title << "\n";
        else std::cout << "Already checked out.\n";
    }

    void return_book(const std::string& title) {
        Book* b = find_by_title(title);
        if (!b) { std::cout << "Book not found.\n"; return; }
        if (b->return_book()) std::cout << "Returned: " << title << "\n";
        else std::cout << "Was not checked out.\n";
    }

    int available_count() const {
        int c = 0;
        for (const auto& b : books) if (b.is_available()) ++c;
        return c;
    }
};

int main() {
    Library lib;
    lib.add_book(Book("The C++ Programming Language", "Bjarne Stroustrup", "978-0321563842", 2013));
    lib.add_book(Book("Effective Modern C++",         "Scott Meyers",      "978-1491903995", 2014));
    lib.add_book(Book("Clean Code",                   "Robert C. Martin",  "978-0132350884", 2008));
    lib.add_book(Book("Design Patterns",              "Gang of Four",      "978-0201633610", 1994));

    std::cout << "=== Library Catalog ===\n\n";
    lib.list_all();

    std::cout << "\nAvailable: " << lib.available_count() << "\n";

    std::cout << "\nChecking out 'Clean Code'...\n";
    lib.checkout("Clean Code");

    std::cout << "\nTrying to check out 'Clean Code' again...\n";
    lib.checkout("Clean Code");

    std::cout << "\nReturning 'Clean Code'...\n";
    lib.return_book("Clean Code");

    std::cout << "\nFinal catalog:\n";
    lib.list_all();

    return 0;
}
