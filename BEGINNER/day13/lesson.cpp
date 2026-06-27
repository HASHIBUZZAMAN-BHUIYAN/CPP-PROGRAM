// Day 13 — Intro to the STL: containers, iterators, algorithms

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>
#include <string>

int main() {
    std::cout << "=== Day 13: STL Intro ===\n";

    // ============================================================
    // 1. std::vector — dynamic array (you know this from Day 06)
    // ============================================================
    std::cout << "\n--- vector ---\n";
    std::vector<int> v = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    std::sort(v.begin(), v.end());
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // ============================================================
    // 2. std::map — sorted key-value pairs (red-black tree)
    //    O(log n) insert/find/erase
    // ============================================================
    std::cout << "\n--- map ---\n";
    std::map<std::string, int> scores;
    scores["Alice"]   = 95;
    scores["Bob"]     = 82;
    scores["Charlie"] = 91;
    scores["Dave"]    = 78;

    // Iterates in alphabetical order
    for (const auto& [name, score] : scores) {   // C++17 structured bindings
        std::cout << name << ": " << score << "\n";
    }

    // Find
    auto it = scores.find("Bob");
    if (it != scores.end()) std::cout << "Found Bob: " << it->second << "\n";

    // Erase
    scores.erase("Dave");
    std::cout << "After erase Dave: " << scores.size() << " entries\n";

    // ============================================================
    // 3. std::set — sorted unique values, O(log n)
    // ============================================================
    std::cout << "\n--- set ---\n";
    std::set<int> unique = {5, 3, 8, 3, 1, 5, 9};  // duplicates ignored
    for (int x : unique) std::cout << x << " ";     // prints sorted unique
    std::cout << "\n";
    std::cout << "size = " << unique.size() << "  (not 7!)\n";
    std::cout << "count(3) = " << unique.count(3) << "\n";

    // ============================================================
    // 4. std::unordered_map — hash map, O(1) average
    //    no ordering, faster than map for large data
    // ============================================================
    std::cout << "\n--- unordered_map ---\n";
    std::unordered_map<std::string, int> word_count;
    std::string words[] = {"hello", "world", "hello", "cpp", "world", "hello"};
    for (const auto& w : words) ++word_count[w];
    for (const auto& [word, cnt] : word_count)
        std::cout << word << ": " << cnt << "\n";

    // ============================================================
    // 5. std::stack and std::queue (adapters)
    // ============================================================
    std::cout << "\n--- stack (LIFO) ---\n";
    std::stack<int> stk;
    stk.push(1); stk.push(2); stk.push(3);
    while (!stk.empty()) { std::cout << stk.top() << " "; stk.pop(); }
    std::cout << "\n";

    std::cout << "\n--- queue (FIFO) ---\n";
    std::queue<std::string> q;
    q.push("first"); q.push("second"); q.push("third");
    while (!q.empty()) { std::cout << q.front() << " "; q.pop(); }
    std::cout << "\n";

    // ============================================================
    // 6. Key algorithms
    // ============================================================
    std::cout << "\n--- STL algorithms ---\n";
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // std::accumulate (sum, product, etc.)
    int sum = std::accumulate(nums.begin(), nums.end(), 0);
    std::cout << "sum 1-10 = " << sum << "\n";

    int product = std::accumulate(nums.begin(), nums.end(), 1, std::multiplies<int>());
    std::cout << "product 1-10 = " << product << "\n";

    // std::count_if
    int evens = std::count_if(nums.begin(), nums.end(),
                              [](int x){ return x % 2 == 0; });
    std::cout << "even count = " << evens << "\n";

    // std::transform (apply function to each element)
    std::vector<int> squared(nums.size());
    std::transform(nums.begin(), nums.end(), squared.begin(),
                   [](int x){ return x * x; });
    std::cout << "squares: ";
    for (int x : squared) std::cout << x << " ";
    std::cout << "\n";

    // std::find_if
    auto first_gt5 = std::find_if(nums.begin(), nums.end(),
                                  [](int x){ return x > 5; });
    std::cout << "First > 5: " << *first_gt5 << "\n";

    // std::all_of, any_of, none_of
    std::cout << std::boolalpha;
    std::cout << "all > 0:  " << std::all_of(nums.begin(), nums.end(), [](int x){ return x > 0; }) << "\n";
    std::cout << "any > 9:  " << std::any_of(nums.begin(), nums.end(), [](int x){ return x > 9; }) << "\n";
    std::cout << "none < 0: " << std::none_of(nums.begin(), nums.end(),[](int x){ return x < 0; }) << "\n";

    // std::partition
    std::vector<int> data = {1,2,3,4,5,6,7,8,9,10};
    auto pivot = std::partition(data.begin(), data.end(), [](int x){ return x % 2 != 0; });
    std::cout << "Odds:  ";
    for (auto i = data.begin(); i != pivot; ++i) std::cout << *i << " ";
    std::cout << "\nEvens: ";
    for (auto i = pivot; i != data.end(); ++i) std::cout << *i << " ";
    std::cout << "\n";

    // std::min_element, max_element
    auto maxIt = std::max_element(nums.begin(), nums.end());
    auto minIt = std::min_element(nums.begin(), nums.end());
    std::cout << "min=" << *minIt << " max=" << *maxIt << "\n";

    // ============================================================
    // 7. Iterators
    // ============================================================
    std::cout << "\n--- Iterators ---\n";
    std::vector<int> iv = {10, 20, 30, 40, 50};
    for (auto it2 = iv.begin(); it2 != iv.end(); ++it2) {
        std::cout << *it2 << " ";
    }
    std::cout << "\n";
    // Reverse iterator
    for (auto rit = iv.rbegin(); rit != iv.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\n";

    return 0;
}
