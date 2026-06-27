// INTERMEDIATE Day 02 — STL Containers Deep Dive

#include <iostream>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

// Simple timer for measuring container operations
struct Timer {
    std::chrono::high_resolution_clock::time_point start;
    Timer() : start(std::chrono::high_resolution_clock::now()) {}
    double ms() const {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};

int main() {
    std::cout << "=== INTERMEDIATE Day 02: STL Containers Deep Dive ===\n";

    // ============================================================
    // std::list — doubly-linked list
    // Good for: O(1) insert/delete anywhere when you have an iterator
    // Bad for: random access (O(n)), cache unfriendly
    // ============================================================
    std::cout << "\n--- std::list ---\n";
    std::list<int> lst = {3, 1, 4, 1, 5, 9, 2, 6};

    lst.push_front(0);       // O(1) — this is the list advantage over vector
    lst.push_back(7);

    // Insert in the middle
    auto it = lst.begin();
    std::advance(it, 3);     // advance by 3 positions
    lst.insert(it, 99);

    std::cout << "list: ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "size=" << lst.size() << " front=" << lst.front() << " back=" << lst.back() << "\n";

    // Remove specific value
    lst.remove(1);  // removes ALL occurrences of 1
    std::cout << "After remove(1): ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";

    // Sort and unique (same name, different behavior than vector)
    lst.sort();
    lst.unique();  // removes consecutive duplicates
    std::cout << "After sort+unique: ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";

    // ============================================================
    // std::set — sorted unique values (red-black tree), O(log n)
    // ============================================================
    std::cout << "\n--- std::set ---\n";
    std::set<std::string> cities = {"London", "Paris", "Tokyo", "London", "Berlin"};
    // Note: "London" appears only once (it's a set)
    for (const auto& c : cities) std::cout << c << " ";
    std::cout << "\n";
    std::cout << "size=" << cities.size() << "\n";

    cities.insert("Madrid");
    auto [ins_it, inserted] = cities.insert("London");  // C++17 structured bindings
    std::cout << "Insert 'London' again: inserted=" << std::boolalpha << inserted << "\n";

    // lower_bound / upper_bound — unique to sorted containers
    auto lb = cities.lower_bound("M");
    std::cout << "First city >= 'M': " << *lb << "\n";

    // ============================================================
    // std::multiset — allows duplicate values
    // ============================================================
    std::cout << "\n--- std::multiset ---\n";
    std::multiset<int> ms = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    for (int x : ms) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "count(5) = " << ms.count(5) << "  count(1) = " << ms.count(1) << "\n";

    // ============================================================
    // std::unordered_map — hash map, O(1) avg, no ordering
    // ============================================================
    std::cout << "\n--- unordered_map vs map ---\n";
    const int N = 100000;

    std::map<int,int> ordered_map;
    std::unordered_map<int,int> hash_map;

    {
        Timer t;
        for (int i = 0; i < N; ++i) ordered_map[i] = i * i;
        std::cout << "map      insert " << N << " items: " << t.ms() << " ms\n";
    }
    {
        Timer t;
        for (int i = 0; i < N; ++i) hash_map[i] = i * i;
        std::cout << "unord_map insert " << N << " items: " << t.ms() << " ms\n";
    }
    {
        Timer t;
        long long sum = 0;
        for (int i = 0; i < N; ++i) sum += ordered_map[i];
        std::cout << "map      lookup  " << N << " items: " << t.ms() << " ms\n";
    }
    {
        Timer t;
        long long sum = 0;
        for (int i = 0; i < N; ++i) sum += hash_map[i];
        std::cout << "unord_map lookup " << N << " items: " << t.ms() << " ms\n";
    }

    // ============================================================
    // std::deque — double-ended queue, O(1) push/pop at both ends
    // ============================================================
    std::cout << "\n--- std::deque ---\n";
    std::deque<int> dq = {3, 4, 5};
    dq.push_front(2);  // O(1) — unlike vector which is O(n) for front insert
    dq.push_front(1);
    dq.push_back(6);
    for (int x : dq) std::cout << x << " ";
    std::cout << "\n";
    dq.pop_front();
    dq.pop_back();
    for (int x : dq) std::cout << x << " ";
    std::cout << "\n";

    // ============================================================
    // Choosing the right container — summary
    // ============================================================
    std::cout << "\n--- Container selection guide ---\n";
    std::cout << "vector:       sequential, fast index access, rare insert/delete mid\n";
    std::cout << "list:         frequent insert/delete mid, no random access needed\n";
    std::cout << "deque:        fast insert/delete at both ends\n";
    std::cout << "set:          sorted unique keys, O(log n)\n";
    std::cout << "multiset:     sorted non-unique values\n";
    std::cout << "map:          sorted key-value, O(log n)\n";
    std::cout << "unordered_map: fastest lookup/insert, no order, O(1) avg\n";
    std::cout << "unordered_set: fast unique membership test, O(1) avg\n";

    return 0;
}
