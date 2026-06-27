#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <chrono>

enum class TaskStatus { TODO, IN_PROGRESS, DONE };
enum class TaskPriority { LOW, MEDIUM, HIGH };
enum class TaskType { BUG, FEATURE, CHORE };

std::string to_string(TaskStatus s);
std::string to_string(TaskPriority p);
std::string to_string(TaskType t);

class Task {
protected:
    int         id;
    std::string title;
    std::string description;
    TaskStatus  status;
    TaskPriority priority;
    TaskType    type;
    std::vector<std::string> tags;

    using ChangeHandler = std::function<void(const Task&, const std::string& event)>;
    std::vector<ChangeHandler> observers;

    void notify(const std::string& event) const;

public:
    Task(int id, const std::string& title, TaskType type, TaskPriority priority = TaskPriority::MEDIUM);
    virtual ~Task() {}

    int          get_id()          const { return id; }
    const std::string& get_title() const { return title; }
    TaskStatus   get_status()      const { return status; }
    TaskPriority get_priority()    const { return priority; }
    TaskType     get_type()        const { return type; }
    const std::string& get_desc()  const { return description; }

    void set_description(const std::string& d) { description = d; }
    void set_status(TaskStatus s);
    void add_tag(const std::string& tag) { tags.push_back(tag); }
    void subscribe(ChangeHandler h)      { observers.push_back(h); }

    virtual void print() const;
    virtual std::string type_label() const = 0;
};

class BugTask : public Task {
    std::string steps_to_reproduce;
    int severity;
public:
    BugTask(int id, const std::string& title, int severity = 3);
    void set_steps(const std::string& s) { steps_to_reproduce = s; }
    std::string type_label() const override { return "BUG"; }
    void print() const override;
};

class FeatureTask : public Task {
    std::string user_story;
    int story_points;
public:
    FeatureTask(int id, const std::string& title, int points = 3);
    void set_user_story(const std::string& s) { user_story = s; }
    std::string type_label() const override { return "FEATURE"; }
    void print() const override;
};

class ChoreTask : public Task {
    bool automated;
public:
    ChoreTask(int id, const std::string& title, bool automated = false);
    std::string type_label() const override { return "CHORE"; }
};
