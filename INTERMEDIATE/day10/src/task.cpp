#include "task.h"
#include <iostream>
#include <iomanip>

std::string to_string(TaskStatus s) {
    switch (s) {
        case TaskStatus::TODO:        return "TODO";
        case TaskStatus::IN_PROGRESS: return "IN_PROGRESS";
        case TaskStatus::DONE:        return "DONE";
    }
    return "";
}
std::string to_string(TaskPriority p) {
    switch (p) {
        case TaskPriority::LOW:    return "Low";
        case TaskPriority::MEDIUM: return "Medium";
        case TaskPriority::HIGH:   return "High";
    }
    return "";
}
std::string to_string(TaskType t) {
    switch (t) {
        case TaskType::BUG:     return "Bug";
        case TaskType::FEATURE: return "Feature";
        case TaskType::CHORE:   return "Chore";
    }
    return "";
}

void Task::notify(const std::string& event) const {
    for (const auto& h : observers) h(*this, event);
}

Task::Task(int id, const std::string& title, TaskType type, TaskPriority priority)
    : id(id), title(title), status(TaskStatus::TODO), priority(priority), type(type) {}

void Task::set_status(TaskStatus s) {
    status = s;
    notify("status_changed");
}

void Task::print() const {
    std::cout << std::left
              << "[" << std::setw(7) << type_label() << "] "
              << "#" << std::setw(3) << id << " "
              << std::setw(30) << title
              << " | " << std::setw(11) << to_string(status)
              << " | " << std::setw(6) << to_string(priority)
              << "\n";
}

BugTask::BugTask(int id, const std::string& title, int severity)
    : Task(id, title, TaskType::BUG), severity(severity) {}

void BugTask::print() const {
    Task::print();
    if (!steps_to_reproduce.empty())
        std::cout << "         Steps: " << steps_to_reproduce << "\n";
}

FeatureTask::FeatureTask(int id, const std::string& title, int points)
    : Task(id, title, TaskType::FEATURE), story_points(points) {}

void FeatureTask::print() const {
    Task::print();
    if (!user_story.empty())
        std::cout << "         Story: " << user_story << "\n";
}

ChoreTask::ChoreTask(int id, const std::string& title, bool automated)
    : Task(id, title, TaskType::CHORE), automated(automated) {}
