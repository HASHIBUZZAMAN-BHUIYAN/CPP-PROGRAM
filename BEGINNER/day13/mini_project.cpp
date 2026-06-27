// Day 13 Mini Project — Task Manager using STL

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

enum class Priority { LOW = 1, MEDIUM = 2, HIGH = 3 };

std::string priority_name(Priority p) {
    switch (p) {
        case Priority::LOW:    return "Low";
        case Priority::MEDIUM: return "Medium";
        case Priority::HIGH:   return "High";
    }
    return "";
}

struct Task {
    int id;
    std::string title, category;
    Priority priority;
    bool done;
};

class TaskManager {
    std::map<int, Task> tasks;
    int next_id = 1;

public:
    int add(const std::string& title, const std::string& cat, Priority p) {
        int id = next_id++;
        tasks[id] = {id, title, cat, p, false};
        return id;
    }

    void complete(int id) {
        auto it = tasks.find(id);
        if (it != tasks.end()) it->second.done = true;
    }

    void list(bool show_done = true) const {
        std::vector<Task> sorted;
        for (const auto& [id, t] : tasks) {
            if (show_done || !t.done) sorted.push_back(t);
        }
        std::sort(sorted.begin(), sorted.end(), [](const Task& a, const Task& b){
            return static_cast<int>(a.priority) > static_cast<int>(b.priority);
        });
        std::cout << std::left << std::setw(4) << "ID"
                  << std::setw(30) << "Title"
                  << std::setw(12) << "Category"
                  << std::setw(8) << "Priority"
                  << "Done\n";
        std::cout << std::string(60, '-') << "\n";
        for (const auto& t : sorted) {
            std::cout << std::setw(4) << t.id
                      << std::setw(30) << t.title
                      << std::setw(12) << t.category
                      << std::setw(8) << priority_name(t.priority)
                      << (t.done ? "Yes" : "No") << "\n";
        }
    }

    std::map<std::string, int> stats() const {
        std::map<std::string, int> counts;
        for (const auto& [id, t] : tasks)
            ++counts[t.category];
        return counts;
    }
};

int main() {
    TaskManager tm;

    tm.add("Write unit tests",     "Development", Priority::HIGH);
    tm.add("Fix login bug",        "Development", Priority::HIGH);
    tm.add("Update README",        "Docs",        Priority::MEDIUM);
    tm.add("Code review PR #42",   "Review",      Priority::MEDIUM);
    tm.add("Deploy to staging",    "DevOps",      Priority::LOW);
    tm.add("Write API docs",       "Docs",        Priority::LOW);

    std::cout << "=== Task Manager ===\n\nAll tasks (sorted by priority):\n";
    tm.list();

    std::cout << "\nMarking tasks 1 and 3 as done...\n";
    tm.complete(1);
    tm.complete(3);

    std::cout << "\nPending tasks only:\n";
    tm.list(false);

    std::cout << "\nTasks by category:\n";
    for (const auto& [cat, cnt] : tm.stats())
        std::cout << "  " << std::setw(15) << cat << ": " << cnt << "\n";

    return 0;
}
