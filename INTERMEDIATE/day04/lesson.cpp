// INTERMEDIATE Day 04 — Smart Pointers: unique_ptr, shared_ptr, weak_ptr

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================
// Why smart pointers?
// Raw pointers require manual delete — easy to forget, exception-unsafe.
// Smart pointers automate ownership and cleanup via RAII.
// ============================================================

class Resource {
    std::string name;
public:
    Resource(const std::string& n) : name(n) {
        std::cout << "[Resource '" << name << "' acquired]\n";
    }
    ~Resource() {
        std::cout << "[Resource '" << name << "' released]\n";
    }
    void use() const { std::cout << "Using " << name << "\n"; }
    const std::string& get_name() const { return name; }
};

// ============================================================
// unique_ptr — SOLE ownership
// Move-only (no copy). Deletes when it goes out of scope.
// Use for: 90% of pointer use cases
// ============================================================
void demo_unique_ptr() {
    std::cout << "\n--- unique_ptr ---\n";

    // Create — prefer make_unique (C++14)
    auto r1 = std::make_unique<Resource>("R1");
    r1->use();

    // Transfer ownership with std::move
    std::unique_ptr<Resource> r2 = std::move(r1);
    // r1 is now null
    if (!r1) std::cout << "r1 is now null (ownership transferred)\n";
    r2->use();

    // Can't copy a unique_ptr — enforces single ownership
    // auto r3 = r1;  // compile error

    // unique_ptr in a function — automatic cleanup on return (even on exception)
    auto allocate = [](bool fail) -> std::unique_ptr<Resource> {
        auto r = std::make_unique<Resource>("TempR");
        if (fail) throw std::runtime_error("simulated failure");
        return r;  // move semantics — no copy
    };

    try {
        auto r = allocate(false);
        r->use();
    } catch (...) {}
    // r released here automatically

    // unique_ptr for arrays
    auto arr = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) arr[i] = i * i;
    std::cout << "Array: ";
    for (int i = 0; i < 5; ++i) std::cout << arr[i] << " ";
    std::cout << "\n";

    std::cout << "[unique_ptr demo ending — r2 will be released now]\n";
}

// ============================================================
// shared_ptr — SHARED ownership (reference counted)
// Multiple pointers to the same object. Deletes when last owner dies.
// Use for: shared ownership, returning from functions that need shared data
// Overhead: atomic reference count (slightly more expensive than unique_ptr)
// ============================================================
void demo_shared_ptr() {
    std::cout << "\n--- shared_ptr ---\n";

    auto s1 = std::make_shared<Resource>("Shared");
    std::cout << "use_count after s1: " << s1.use_count() << "\n";

    {
        std::shared_ptr<Resource> s2 = s1;  // both own it now
        std::cout << "use_count after s2: " << s1.use_count() << "\n";
        s2->use();
    }
    // s2 goes out of scope — but Resource is NOT released yet (s1 still exists)
    std::cout << "use_count after s2 scope: " << s1.use_count() << "\n";

    // shared_ptr in a vector
    std::vector<std::shared_ptr<Resource>> pool;
    pool.push_back(s1);
    pool.push_back(std::make_shared<Resource>("A"));
    pool.push_back(std::make_shared<Resource>("B"));

    std::cout << "Pool use_counts: ";
    for (const auto& p : pool) std::cout << p.use_count() << " ";
    std::cout << "\n";

    std::cout << "[shared_ptr demo ending — Shared released when s1 + pool[0] both die]\n";
}

// ============================================================
// weak_ptr — NON-OWNING reference to a shared_ptr object
// Breaks cycles (a shared_ptr cycle would leak memory).
// Must .lock() to get a temporary shared_ptr before using.
// ============================================================
struct Node {
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node>   prev;   // <-- weak to break the cycle

    Node(const std::string& n) : name(n) {
        std::cout << "[Node '" << name << "' created]\n";
    }
    ~Node() {
        std::cout << "[Node '" << name << "' destroyed]\n";
    }
};

void demo_weak_ptr() {
    std::cout << "\n--- weak_ptr (breaking cycles) ---\n";

    auto n1 = std::make_shared<Node>("First");
    auto n2 = std::make_shared<Node>("Second");

    n1->next = n2;
    n2->prev = n1;  // weak_ptr — doesn't increase ref count

    std::cout << "n1 use_count: " << n1.use_count() << "  (only n1 owns 'First')\n";
    std::cout << "n2 use_count: " << n2.use_count() << "  (n1->next + n2 own 'Second')\n";

    // Access via weak_ptr::lock
    if (auto p = n2->prev.lock()) {  // .lock() returns shared_ptr or nullptr
        std::cout << "n2->prev = '" << p->name << "'\n";
    }

    std::cout << "[weak_ptr demo ending — both Nodes will be destroyed properly]\n";
}

int main() {
    std::cout << "=== INTERMEDIATE Day 04: Smart Pointers ===\n";

    demo_unique_ptr();
    demo_shared_ptr();
    demo_weak_ptr();

    std::cout << "\n--- Summary ---\n";
    std::cout << "unique_ptr : one owner, zero overhead, prefer this by default\n";
    std::cout << "shared_ptr : multiple owners, reference counted, small overhead\n";
    std::cout << "weak_ptr   : non-owning observer, use to break shared_ptr cycles\n";
    std::cout << "raw ptr    : use only as non-owning observer (like a reference)\n";
    std::cout << "             NEVER use raw ptr for ownership in modern C++\n";

    return 0;
}
