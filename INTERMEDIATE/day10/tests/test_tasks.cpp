// Simple manual tests for the task system (no external framework needed)

#include "task.h"
#include <iostream>
#include <cassert>

int pass_count = 0, fail_count = 0;

#define CHECK(expr) do { \
    if (expr) { ++pass_count; std::cout << "PASS: " #expr "\n"; } \
    else { ++fail_count; std::cout << "FAIL: " #expr " at line " << __LINE__ << "\n"; } \
} while(0)

#define CHECK_THROWS(expr) do { \
    bool threw = false; try { expr; } catch (...) { threw = true; } \
    if (threw) { ++pass_count; std::cout << "PASS: " #expr " throws\n"; } \
    else { ++fail_count; std::cout << "FAIL: " #expr " did not throw\n"; } \
} while(0)

int main() {
    std::cout << "=== Task Tests ===\n\n";

    // Test BugTask creation
    BugTask bug(1, "Login crash", 5);
    CHECK(bug.get_id()       == 1);
    CHECK(bug.get_title()    == "Login crash");
    CHECK(bug.get_status()   == TaskStatus::TODO);
    CHECK(bug.get_type()     == TaskType::BUG);
    CHECK(bug.type_label()   == "BUG");

    // Test status transitions
    bug.set_status(TaskStatus::IN_PROGRESS);
    CHECK(bug.get_status() == TaskStatus::IN_PROGRESS);
    bug.set_status(TaskStatus::DONE);
    CHECK(bug.get_status() == TaskStatus::DONE);

    // Test FeatureTask
    FeatureTask feat(2, "Dark mode", 5);
    CHECK(feat.get_type()   == TaskType::FEATURE);
    CHECK(feat.type_label() == "FEATURE");
    CHECK(feat.get_status() == TaskStatus::TODO);

    // Test ChoreTask
    ChoreTask chore(3, "Update deps");
    CHECK(chore.get_type() == TaskType::CHORE);

    // Test observer notification
    int notify_count = 0;
    BugTask bug2(4, "Another bug");
    bug2.subscribe([&notify_count](const Task&, const std::string&) { ++notify_count; });
    bug2.set_status(TaskStatus::IN_PROGRESS);
    bug2.set_status(TaskStatus::DONE);
    CHECK(notify_count == 2);

    // Test to_string functions
    CHECK(to_string(TaskStatus::TODO)        == "TODO");
    CHECK(to_string(TaskStatus::IN_PROGRESS) == "IN_PROGRESS");
    CHECK(to_string(TaskStatus::DONE)        == "DONE");
    CHECK(to_string(TaskPriority::HIGH)      == "High");
    CHECK(to_string(TaskType::BUG)           == "Bug");

    std::cout << "\n" << pass_count << " passed, " << fail_count << " failed.\n";
    return fail_count > 0 ? 1 : 0;
}
