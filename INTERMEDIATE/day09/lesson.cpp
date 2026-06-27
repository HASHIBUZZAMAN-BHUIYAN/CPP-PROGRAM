// INTERMEDIATE Day 09 — Design Patterns II: Observer, Strategy, RAII

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <map>
#include <chrono>

// ============================================================
// PATTERN 4: Observer (Event/Notification pattern)
// Objects subscribe to events and get notified automatically.
// Use for: UI events, game events, pub-sub messaging, MVC.
// ============================================================
class EventSystem {
public:
    using Handler = std::function<void(const std::string& data)>;

private:
    std::map<std::string, std::vector<Handler>> listeners;
    std::map<std::string, std::vector<Handler>> oneshot;

public:
    void on(const std::string& event, Handler h) {
        listeners[event].push_back(h);
    }

    void once(const std::string& event, Handler h) {
        oneshot[event].push_back(h);
    }

    void emit(const std::string& event, const std::string& data = "") {
        if (listeners.count(event)) {
            for (const auto& h : listeners[event]) h(data);
        }
        if (oneshot.count(event)) {
            for (const auto& h : oneshot[event]) h(data);
            oneshot.erase(event);  // one-shot: fires once then gone
        }
    }
};

// Classic OOP observer pattern
class IObserver {
public:
    virtual ~IObserver() {}
    virtual void update(const std::string& msg) = 0;
};

class EventLog : public IObserver {
    std::string name;
public:
    EventLog(const std::string& n) : name(n) {}
    void update(const std::string& msg) override {
        std::cout << "  [" << name << "] received: " << msg << "\n";
    }
};

class Observable {
    std::vector<IObserver*> observers;

public:
    void subscribe(IObserver* o)   { observers.push_back(o); }
    void unsubscribe(IObserver* o) {
        observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
    }
    void notify(const std::string& msg) {
        for (auto* o : observers) o->update(msg);
    }
};

// ============================================================
// PATTERN 5: Strategy
// Defines a family of algorithms, makes them interchangeable.
// Use for: sorting strategies, payment methods, compression, serialization.
// ============================================================
class SortStrategy {
public:
    virtual ~SortStrategy() {}
    virtual void sort(std::vector<int>& data) = 0;
    virtual std::string name() const = 0;
};

class BubbleSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override {
        int n = data.size();
        for (int i = 0; i < n-1; ++i)
            for (int j = 0; j < n-i-1; ++j)
                if (data[j] > data[j+1]) std::swap(data[j], data[j+1]);
    }
    std::string name() const override { return "BubbleSort"; }
};

class QuickSort : public SortStrategy {
    void qs(std::vector<int>& d, int lo, int hi) {
        if (lo >= hi) return;
        int pivot = d[hi], i = lo - 1;
        for (int j = lo; j < hi; ++j) if (d[j] <= pivot) std::swap(d[++i], d[j]);
        std::swap(d[i+1], d[hi]);
        int p = i + 1;
        qs(d, lo, p-1); qs(d, p+1, hi);
    }
public:
    void sort(std::vector<int>& data) override { if (!data.empty()) qs(data, 0, data.size()-1); }
    std::string name() const override { return "QuickSort"; }
};

class StdSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override { std::sort(data.begin(), data.end()); }
    std::string name() const override { return "std::sort"; }
};

class Sorter {
    std::unique_ptr<SortStrategy> strategy;

public:
    void set_strategy(std::unique_ptr<SortStrategy> s) { strategy = std::move(s); }
    std::vector<int> sort_copy(std::vector<int> data) {
        strategy->sort(data);
        return data;
    }
    std::string strategy_name() const { return strategy ? strategy->name() : "none"; }
};

// Modern strategy with lambdas (no class hierarchy needed)
using Predicate = std::function<bool(int, int)>;

void flexible_sort(std::vector<int>& v, Predicate compare) {
    std::sort(v.begin(), v.end(), compare);
}

// ============================================================
// PATTERN 6: RAII (Resource Acquisition Is Initialization)
// Tie resource lifetime to object lifetime — the most C++ pattern.
// Use for: file handles, mutexes, connections, any resource.
// ============================================================
class FileGuard {
    std::ofstream file;
    std::string   filename;

public:
    FileGuard(const std::string& name, std::ios::openmode mode = std::ios::out)
        : file(name, mode), filename(name)
    {
        if (!file) throw std::runtime_error("Cannot open: " + name);
        std::cout << "[FileGuard opened '" << name << "']\n";
    }

    ~FileGuard() {
        if (file.is_open()) file.close();
        std::cout << "[FileGuard closed '" << filename << "']\n";
    }

    // Non-copyable (a file has one owner)
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    void write(const std::string& s) { file << s; }
    bool ok() const { return file.good(); }
};

class ScopedTimer {
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
public:
    ScopedTimer(const std::string& n)
        : name(n), start(std::chrono::high_resolution_clock::now())
    {
        std::cout << "[Timer '" << name << "' started]\n";
    }
    ~ScopedTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "[Timer '" << name << "' ended: " << ms << " ms]\n";
    }
};

int main() {
    std::cout << "=== INTERMEDIATE Day 09: Design Patterns II ===\n";

    // --- Observer (functional style) ---
    std::cout << "\n--- Observer: EventSystem ---\n";
    EventSystem events;

    events.on("login",  [](const std::string& user){ std::cout << "  Log: " << user << " logged in\n"; });
    events.on("login",  [](const std::string& user){ std::cout << "  Mail: Welcome back, " << user << "\n"; });
    events.once("login",[](const std::string& user){ std::cout << "  ONE-SHOT: First login of " << user << "\n"; });

    events.emit("login", "Alice");
    std::cout << "  (second login — one-shot is gone)\n";
    events.emit("login", "Bob");

    // --- Observer (OOP style) ---
    std::cout << "\n--- Observer: Classic OOP ---\n";
    Observable source;
    EventLog log1("Logger"), log2("Monitor");
    source.subscribe(&log1);
    source.subscribe(&log2);
    source.notify("System started");
    source.unsubscribe(&log1);
    source.notify("Config loaded");  // only log2 receives this

    // --- Strategy ---
    std::cout << "\n--- Strategy: Sorting ---\n";
    std::vector<int> data = {5, 3, 8, 1, 9, 2, 7, 4, 6};

    Sorter sorter;
    // Can't use initializer_list with unique_ptr (no copy), so test each explicitly
    sorter.set_strategy(std::make_unique<BubbleSort>());
    auto s1 = sorter.sort_copy(data);
    std::cout << sorter.strategy_name() << ": ";
    for (int x : s1) std::cout << x << " ";
    std::cout << "\n";

    sorter.set_strategy(std::make_unique<QuickSort>());
    auto s2 = sorter.sort_copy(data);
    std::cout << sorter.strategy_name() << ": ";
    for (int x : s2) std::cout << x << " ";
    std::cout << "\n";

    sorter.set_strategy(std::make_unique<StdSort>());
    auto s3 = sorter.sort_copy(data);
    std::cout << sorter.strategy_name() << ": ";
    for (int x : s3) std::cout << x << " ";
    std::cout << "\n";

    // Lambda strategy
    std::vector<int> v = data;
    flexible_sort(v, [](int a, int b){ return a > b; });  // descending
    std::cout << "Lambda desc: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // --- RAII ---
    std::cout << "\n--- RAII ---\n";
    {
        FileGuard fg("raii_demo.txt");
        fg.write("Hello from RAII!\n");
        fg.write("The file will close automatically.\n");
        // Even if an exception occurs here, ~FileGuard() runs
    }  // <- file closed here

    {
        ScopedTimer timer("work block");
        volatile long long sum = 0;
        for (int i = 0; i < 10000000; ++i) sum += i;
        std::cout << "  (computed sum=" << sum << ")\n";
    }  // <- timer prints elapsed time here

    try {
        FileGuard bad("C:/this/path/does/not/exist/file.txt");
    } catch (const std::exception& e) {
        std::cout << "Caught expected: " << e.what() << "\n";
    }

    return 0;
}
