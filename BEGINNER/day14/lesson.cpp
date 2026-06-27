// Day 14 — Capstone: Contact Manager
// Combines: classes, STL (map/vector), file I/O, exceptions, operator overloading

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>

// ============================================================
// Contact class
// ============================================================
class Contact {
private:
    std::string name, phone, email;

public:
    Contact(const std::string& n, const std::string& p, const std::string& e)
        : name(n), phone(p), email(e)
    {
        if (name.empty()) throw std::invalid_argument("Name cannot be empty");
    }

    const std::string& get_name()  const { return name;  }
    const std::string& get_phone() const { return phone; }
    const std::string& get_email() const { return email; }

    void set_phone(const std::string& p) { phone = p; }
    void set_email(const std::string& e) { email = e; }

    bool operator<(const Contact& rhs) const { return name < rhs.name; }

    void print() const {
        std::cout << std::left
                  << std::setw(20) << name
                  << std::setw(16) << phone
                  << email << "\n";
    }

    std::string to_csv() const {
        return name + "," + phone + "," + email;
    }

    static Contact from_csv(const std::string& line) {
        std::istringstream iss(line);
        std::string n, p, e;
        std::getline(iss, n, ',');
        std::getline(iss, p, ',');
        std::getline(iss, e, ',');
        return Contact(n, p, e);
    }
};

// ============================================================
// ContactBook class
// ============================================================
class ContactBook {
private:
    std::map<std::string, Contact> contacts;

public:
    void add(const Contact& c) {
        if (contacts.count(c.get_name()))
            throw std::runtime_error("Contact already exists: " + c.get_name());
        contacts.emplace(c.get_name(), c);
    }

    void remove(const std::string& name) {
        if (!contacts.erase(name))
            throw std::runtime_error("Contact not found: " + name);
    }

    Contact& get(const std::string& name) {
        auto it = contacts.find(name);
        if (it == contacts.end())
            throw std::runtime_error("Contact not found: " + name);
        return it->second;
    }

    std::vector<Contact> search(const std::string& query) const {
        std::vector<Contact> results;
        std::string q = query;
        std::transform(q.begin(), q.end(), q.begin(), ::tolower);
        for (const auto& [k, c] : contacts) {
            std::string lname = c.get_name();
            std::transform(lname.begin(), lname.end(), lname.begin(), ::tolower);
            if (lname.find(q) != std::string::npos)
                results.push_back(c);
        }
        return results;
    }

    void list_all() const {
        if (contacts.empty()) { std::cout << "(no contacts)\n"; return; }
        std::cout << std::left << std::setw(20) << "Name"
                  << std::setw(16) << "Phone" << "Email\n";
        std::cout << std::string(60, '-') << "\n";
        for (const auto& [k, c] : contacts) c.print();
    }

    size_t size() const { return contacts.size(); }

    void save_to_file(const std::string& filename) const {
        std::ofstream f(filename);
        if (!f) throw std::runtime_error("Cannot open file for writing: " + filename);
        for (const auto& [k, c] : contacts) f << c.to_csv() << "\n";
        std::cout << "Saved " << contacts.size() << " contacts to " << filename << "\n";
    }

    void load_from_file(const std::string& filename) {
        std::ifstream f(filename);
        if (!f) throw std::runtime_error("Cannot open file: " + filename);
        std::string line;
        int loaded = 0;
        while (std::getline(f, line)) {
            if (line.empty()) continue;
            try {
                add(Contact::from_csv(line));
                ++loaded;
            } catch (...) {}
        }
        std::cout << "Loaded " << loaded << " contacts from " << filename << "\n";
    }
};

// ============================================================
// Interactive menu
// ============================================================
void show_menu() {
    std::cout << "\n=== Contact Manager ===\n"
              << "1. Add contact\n"
              << "2. Remove contact\n"
              << "3. Search contact\n"
              << "4. List all\n"
              << "5. Save to file\n"
              << "6. Load from file\n"
              << "7. Edit phone/email\n"
              << "0. Quit\n"
              << "Choice: ";
}

int main() {
    ContactBook book;

    // Pre-load some contacts
    try {
        book.add(Contact("Alice Smith", "555-0101", "alice@example.com"));
        book.add(Contact("Bob Jones",   "555-0102", "bob@example.com"));
        book.add(Contact("Carol White", "555-0103", "carol@example.com"));
    } catch (const std::exception& e) {
        std::cerr << "Init error: " << e.what() << "\n";
    }

    int choice = -1;
    while (choice != 0) {
        show_menu();
        std::cin >> choice;
        std::cin.ignore();

        try {
            if (choice == 1) {
                std::string name, phone, email;
                std::cout << "Name: ";  std::getline(std::cin, name);
                std::cout << "Phone: "; std::getline(std::cin, phone);
                std::cout << "Email: "; std::getline(std::cin, email);
                book.add(Contact(name, phone, email));
                std::cout << "Added.\n";

            } else if (choice == 2) {
                std::string name;
                std::cout << "Name to remove: ";
                std::getline(std::cin, name);
                book.remove(name);
                std::cout << "Removed.\n";

            } else if (choice == 3) {
                std::string query;
                std::cout << "Search query: ";
                std::getline(std::cin, query);
                auto results = book.search(query);
                if (results.empty()) std::cout << "No results.\n";
                else for (const auto& c : results) c.print();

            } else if (choice == 4) {
                book.list_all();
                std::cout << "Total: " << book.size() << "\n";

            } else if (choice == 5) {
                std::string fn;
                std::cout << "Filename: "; std::getline(std::cin, fn);
                book.save_to_file(fn);

            } else if (choice == 6) {
                std::string fn;
                std::cout << "Filename: "; std::getline(std::cin, fn);
                book.load_from_file(fn);

            } else if (choice == 7) {
                std::string name, phone, email;
                std::cout << "Name: "; std::getline(std::cin, name);
                auto& c = book.get(name);
                std::cout << "New phone (blank to keep): "; std::getline(std::cin, phone);
                std::cout << "New email (blank to keep): "; std::getline(std::cin, email);
                if (!phone.empty()) c.set_phone(phone);
                if (!email.empty()) c.set_email(email);
                std::cout << "Updated.\n";

            } else if (choice != 0) {
                std::cout << "Invalid choice.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
