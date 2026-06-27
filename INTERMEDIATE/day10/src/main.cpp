#include "task.h"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

// Factory function
std::unique_ptr<Task> create_task(const std::string& type, int id, const std::string& title) {
    if      (type == "bug")     return std::make_unique<BugTask>(id, title);
    else if (type == "feature") return std::make_unique<FeatureTask>(id, title);
    else if (type == "chore")   return std::make_unique<ChoreTask>(id, title);
    throw std::invalid_argument("Unknown type: " + type);
}

int main() {
    std::cout << "=== INTERMEDIATE Day 10: Task Manager Capstone ===\n\n";

    std::vector<std::unique_ptr<Task>> tasks;
    int next_id = 1;

    // Observer: print notifications
    auto notifier = [](const Task& t, const std::string& event) {
        std::cout << "  >> Event '" << event << "' on task #" << t.get_id()
                  << " '" << t.get_title() << "'\n";
    };

    // Create tasks using factory pattern
    auto t1 = create_task("bug", next_id++, "Login page crashes on empty email");
    auto t2 = create_task("feature", next_id++, "Add dark mode");
    auto t3 = create_task("bug", next_id++, "Avatar upload silently fails");
    auto t4 = create_task("chore", next_id++, "Update dependencies");
    auto t5 = create_task("feature", next_id++, "Export to PDF");

    // Subscribe observers
    t1->subscribe(notifier);
    t2->subscribe(notifier);

    // Mutate state (triggers observers for t1 and t2)
    t1->set_status(TaskStatus::IN_PROGRESS);
    t2->set_status(TaskStatus::DONE);
    t3->set_status(TaskStatus::DONE);
    t4->set_status(TaskStatus::IN_PROGRESS);

    tasks.push_back(std::move(t1));
    tasks.push_back(std::move(t2));
    tasks.push_back(std::move(t3));
    tasks.push_back(std::move(t4));
    tasks.push_back(std::move(t5));

    // List all
    std::cout << "\nAll tasks:\n";
    for (const auto& t : tasks) t->print();

    // Filter by status (lambda predicate)
    auto print_filtered = [&tasks](TaskStatus status, const std::string& label) {
        std::cout << "\n" << label << ":\n";
        for (const auto& t : tasks)
            if (t->get_status() == status) t->print();
    };

    print_filtered(TaskStatus::TODO,        "TODO");
    print_filtered(TaskStatus::IN_PROGRESS, "IN_PROGRESS");
    print_filtered(TaskStatus::DONE,        "DONE");

    // Stats
    int bugs = 0, features = 0, chores = 0;
    for (const auto& t : tasks) {
        switch (t->get_type()) {
            case TaskType::BUG:     ++bugs;     break;
            case TaskType::FEATURE: ++features; break;
            case TaskType::CHORE:   ++chores;   break;
        }
    }
    std::cout << "\nStats: " << bugs << " bugs, " << features
              << " features, " << chores << " chores\n";

    return 0;
}
