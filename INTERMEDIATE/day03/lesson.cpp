// INTERMEDIATE Day 03 — STL Algorithms & Lambdas

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>
#include <iterator>

int main() {
    std::cout << "=== INTERMEDIATE Day 03: STL Algorithms & Lambdas ===\n";

    // ============================================================
    // PART 1: Lambda expressions
    // ============================================================
    std::cout << "\n--- Lambdas ---\n";

    // Basic lambda: [capture](params) -> return_type { body }
    auto add = [](int a, int b) -> int { return a + b; };
    std::cout << "add(3,4) = " << add(3, 4) << "\n";

    // Lambda with capture by value
    int multiplier = 3;
    auto times_n = [multiplier](int x) { return x * multiplier; };
    std::cout << "times 3: 5 -> " << times_n(5) << "\n";

    // Capture by reference (modifies outer variable)
    int count = 0;
    auto increment = [&count]() { ++count; };
    increment(); increment(); increment();
    std::cout << "count after 3 increments: " << count << "\n";

    // Capture all by value [=] or all by reference [&]
    int a = 10, b = 20;
    auto sum_both = [=]() { return a + b; };  // captures a and b by value
    std::cout << "sum_both = " << sum_both() << "\n";

    // Mutable lambda (modifies captured-by-value copies)
    auto counter = [n = 0]() mutable { return ++n; };
    std::cout << counter() << " " << counter() << " " << counter() << "\n";

    // Storing lambdas in std::function
    std::function<double(double)> transform_fn = [](double x) { return x * x; };
    std::cout << "transform_fn(5) = " << transform_fn(5) << "\n";

    // ============================================================
    // PART 2: std::transform
    // ============================================================
    std::cout << "\n--- std::transform ---\n";
    std::vector<int> nums = {1, 2, 3, 4, 5};
    std::vector<int> result(nums.size());

    std::transform(nums.begin(), nums.end(), result.begin(),
                   [](int x) { return x * x; });
    std::cout << "squares: ";
    for (int x : result) std::cout << x << " ";
    std::cout << "\n";

    // Binary transform (two input ranges)
    std::vector<int> a2 = {1, 2, 3}, b2 = {10, 20, 30};
    std::vector<int> c2(3);
    std::transform(a2.begin(), a2.end(), b2.begin(), c2.begin(),
                   [](int x, int y) { return x + y; });
    std::cout << "a+b: ";
    for (int x : c2) std::cout << x << " ";
    std::cout << "\n";

    // In-place transform (same range as output)
    std::vector<std::string> words = {"hello", "world", "cpp"};
    std::transform(words.begin(), words.end(), words.begin(),
                   [](std::string s) {
                       s[0] = std::toupper(s[0]);
                       return s;
                   });
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n";

    // ============================================================
    // PART 3: std::accumulate and friends
    // ============================================================
    std::cout << "\n--- Accumulate variants ---\n";
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Sum
    int total = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "sum: " << total << "\n";

    // Product
    int product = std::accumulate(v.begin(), v.end(), 1,
                                  [](int acc, int x) { return acc * x; });
    std::cout << "product: " << product << "\n";

    // Build a string
    std::string joined = std::accumulate(
        words.begin(), words.end(), std::string(""),
        [](const std::string& acc, const std::string& s) {
            return acc + (acc.empty() ? "" : " ") + s;
        });
    std::cout << "joined: " << joined << "\n";

    // std::reduce (C++17 — like accumulate but order of operations not guaranteed)
    // Great for parallel execution (not shown here, but std::execution::par can parallelize it)
    int r = std::reduce(v.begin(), v.end(), 0, std::plus<int>());
    std::cout << "reduce sum: " << r << "\n";

    // ============================================================
    // PART 4: More algorithms with lambdas
    // ============================================================
    std::cout << "\n--- Algorithms with lambdas ---\n";
    std::vector<int> data = {1, 5, 2, 8, 3, 9, 4, 7, 6};

    // sort with custom comparator
    std::sort(data.begin(), data.end(), std::greater<int>());
    std::cout << "sorted desc: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n";

    // stable_sort — preserves relative order of equal elements
    struct Person { std::string name; int age; };
    std::vector<Person> people = {{"Bob",25},{"Alice",30},{"Carol",25},{"Dave",30}};
    std::stable_sort(people.begin(), people.end(),
                     [](const Person& a, const Person& b){ return a.age < b.age; });
    std::cout << "stable sort by age: ";
    for (const auto& p : people) std::cout << p.name << "(" << p.age << ") ";
    std::cout << "\n";

    // for_each
    std::vector<int> squares;
    std::for_each(data.begin(), data.end(), [&squares](int x){
        squares.push_back(x * x);
    });
    // Note: prefer range-based for or transform for this; for_each is for side effects

    // generate
    std::vector<int> fib(8);
    int prev = 0, curr = 1;
    std::generate(fib.begin(), fib.end(), [&prev, &curr]() {
        int next = prev + curr;
        prev = curr;
        curr = next;
        return prev;
    });
    std::cout << "fibonacci: ";
    for (int x : fib) std::cout << x << " ";
    std::cout << "\n";

    // adjacent_difference (differences between consecutive elements)
    std::vector<int> temps = {20, 23, 21, 25, 22};
    std::vector<int> diffs(temps.size());
    std::adjacent_difference(temps.begin(), temps.end(), diffs.begin());
    std::cout << "temp changes: ";
    for (int x : diffs) std::cout << (x >= 0 ? "+" : "") << x << " ";
    std::cout << "\n";

    // ============================================================
    // PART 5: std::function and higher-order functions
    // ============================================================
    std::cout << "\n--- std::function ---\n";
    using IntToInt = std::function<int(int)>;

    // Function that returns a lambda
    auto make_multiplier = [](int factor) -> IntToInt {
        return [factor](int x) { return x * factor; };
    };

    auto double_it = make_multiplier(2);
    auto triple_it = make_multiplier(3);
    std::cout << "double 5 = " << double_it(5) << "\n";
    std::cout << "triple 5 = " << triple_it(5) << "\n";

    // Pipeline of functions
    std::vector<IntToInt> pipeline = {
        [](int x) { return x + 1; },
        [](int x) { return x * 2; },
        [](int x) { return x - 3; }
    };
    int val = 5;
    for (const auto& fn : pipeline) val = fn(val);
    std::cout << "Pipeline 5 -> " << val << "  (5+1=6, 6*2=12, 12-3=9)\n";

    return 0;
}
