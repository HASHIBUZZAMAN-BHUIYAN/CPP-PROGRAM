// ADVANCED: RAII Resource Management Patterns
//
// WHY THIS MATTERS:
// RAII (Resource Acquisition Is Initialization) is THE core C++ pattern.
// It makes resource management exception-safe without try/finally.
// Every C++ programmer uses RAII constantly; most C++ libraries are built on it.
//
// This file demonstrates:
// 1. A custom RAII wrapper for a simulated file handle
// 2. RAII for a mutex-like lock
// 3. Scope guard (run arbitrary cleanup on scope exit)
// 4. Why RAII beats manual acquire/release in the presence of exceptions

#include <iostream>
#include <string>
#include <stdexcept>
#include <functional>
#include <vector>
#include <memory>

// ============================================================
// Simulated low-level C-style resource (like a file descriptor)
// This models the pattern used when wrapping OS resources.
// ============================================================
namespace SimulatedOS {
    int open_file(const std::string& name) {
        std::cout << "[OS] open('" << name << "') -> handle=42\n";
        if (name == "FAIL") throw std::runtime_error("OS open failed: " + name);
        return 42;
    }
    void close_file(int handle) {
        std::cout << "[OS] close(handle=" << handle << ")\n";
    }
    void write_file(int handle, const std::string& data) {
        std::cout << "[OS] write(handle=" << handle << ", '" << data << "')\n";
    }
    std::string read_file(int handle) {
        return "[data from handle " + std::to_string(handle) + "]";
    }

    int lock_mutex(const std::string& name) {
        std::cout << "[OS] lock('" << name << "')\n";
        return 1;
    }
    void unlock_mutex(int id) {
        std::cout << "[OS] unlock(mutex=" << id << ")\n";
    }
}

// ============================================================
// RAII File Handle
// ============================================================
class FileHandle {
    int         handle;
    std::string name;
    bool        valid;

public:
    explicit FileHandle(const std::string& filename) : valid(false), name(filename) {
        handle = SimulatedOS::open_file(filename);
        valid = true;  // only set if open succeeded (no exception)
    }

    ~FileHandle() {
        if (valid) SimulatedOS::close_file(handle);
    }

    // Non-copyable (a file handle has one owner)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Movable — transfer ownership
    FileHandle(FileHandle&& other) noexcept
        : handle(other.handle), name(other.name), valid(other.valid) {
        other.valid = false;  // source no longer owns it
    }

    void write(const std::string& data) {
        if (!valid) throw std::runtime_error("write to invalid handle");
        SimulatedOS::write_file(handle, data);
    }

    std::string read() {
        if (!valid) throw std::runtime_error("read from invalid handle");
        return SimulatedOS::read_file(handle);
    }

    bool is_valid() const { return valid; }
    const std::string& filename() const { return name; }
};

// ============================================================
// RAII Lock Guard (mirrors std::lock_guard)
// ============================================================
class MutexGuard {
    int  mutex_id;
    bool locked;

public:
    explicit MutexGuard(const std::string& mutex_name) : locked(false) {
        mutex_id = SimulatedOS::lock_mutex(mutex_name);
        locked = true;
    }

    ~MutexGuard() {
        if (locked) SimulatedOS::unlock_mutex(mutex_id);
    }

    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;
};

// ============================================================
// Scope Guard — run arbitrary cleanup at scope exit
// (Like Go's defer, or Java's finally, but RAII-style)
// ============================================================
class ScopeGuard {
    std::function<void()> cleanup;
    bool active;

public:
    explicit ScopeGuard(std::function<void()> fn)
        : cleanup(std::move(fn)), active(true) {}

    ~ScopeGuard() {
        if (active) cleanup();
    }

    void dismiss() { active = false; }  // cancel the cleanup

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

// ============================================================
// Demonstrating RAII vs manual cleanup under exceptions
// ============================================================
void manual_cleanup_demo(bool inject_failure) {
    std::cout << "\n--- Manual cleanup (UNSAFE) ---\n";
    int handle = -1;
    int mutex  = -1;
    try {
        handle = SimulatedOS::open_file("data.txt");
        mutex  = SimulatedOS::lock_mutex("db_mutex");

        SimulatedOS::write_file(handle, "some data");

        if (inject_failure) throw std::runtime_error("simulated failure");

        SimulatedOS::unlock_mutex(mutex);
        SimulatedOS::close_file(handle);
        std::cout << "  All cleaned up (success path)\n";
    } catch (...) {
        // Must remember to clean up in EVERY catch block — easy to forget!
        if (mutex >= 0) SimulatedOS::unlock_mutex(mutex);
        if (handle >= 0) SimulatedOS::close_file(handle);
        std::cout << "  Cleaned up in catch (error path)\n";
    }
}

void raii_cleanup_demo(bool inject_failure) {
    std::cout << "\n--- RAII cleanup (SAFE) ---\n";
    FileHandle fh("data.txt");  // acquired here
    MutexGuard mg("db_mutex"); // acquired here

    fh.write("some data");

    if (inject_failure) {
        throw std::runtime_error("simulated failure");
        // NO CLEANUP CODE NEEDED — destructors run automatically
    }

    std::cout << "  Normal path complete\n";
    // ~MutexGuard() called here — unlocks
    // ~FileHandle() called here — closes
}

int main() {
    std::cout << "=== RAII Resource Management ===\n";

    // --- Basic FileHandle ---
    std::cout << "\n--- FileHandle RAII ---\n";
    {
        FileHandle fh("config.txt");
        fh.write("key=value");
        std::cout << "Read: " << fh.read() << "\n";
    }  // <- ~FileHandle() closes file automatically

    // --- Move semantics ---
    std::cout << "\n--- Move FileHandle ---\n";
    {
        FileHandle fh1("temp.txt");
        FileHandle fh2 = std::move(fh1);  // fh1 no longer owns the handle
        std::cout << "fh1 valid: " << fh1.is_valid() << "\n";
        std::cout << "fh2 valid: " << fh2.is_valid() << "\n";
        fh2.write("moved handle works");
    }  // <- only fh2's ~FileHandle() closes (fh1 is already empty)

    // --- ScopeGuard ---
    std::cout << "\n--- ScopeGuard ---\n";
    {
        ScopeGuard guard([]{ std::cout << "  ScopeGuard cleanup fired!\n"; });
        std::cout << "  Doing work...\n";
    }  // <- cleanup fires here

    {
        ScopeGuard guard([]{ std::cout << "  This should NOT print\n"; });
        std::cout << "  Dismissing guard...\n";
        guard.dismiss();  // cancel the cleanup
    }

    // --- Exception safety comparison ---
    std::cout << "\n=== Exception Safety Comparison ===\n";
    std::cout << "\nSuccess path:\n";
    manual_cleanup_demo(false);
    raii_cleanup_demo(false);

    std::cout << "\nFailure path (exception thrown):\n";
    manual_cleanup_demo(true);
    try {
        raii_cleanup_demo(true);
    } catch (const std::exception& e) {
        std::cout << "  Outer caught: " << e.what() << "\n";
    }
    // RAII version cleans up automatically — no cleanup code in catch needed!

    // --- Open failure ---
    std::cout << "\n--- Failed open is safe ---\n";
    try {
        FileHandle bad("FAIL");
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
        // No close needed — FileHandle::valid was never set to true
    }

    std::cout << "\n=== Key Takeaways ===\n";
    std::cout << "1. RAII ties resource lifetime to object lifetime\n";
    std::cout << "2. Destructor runs automatically — even on exception\n";
    std::cout << "3. Eliminates entire category of resource leak bugs\n";
    std::cout << "4. std::unique_ptr, lock_guard, ifstream are all RAII\n";
    std::cout << "5. This pattern doesn't exist in Java/Python (they need finally/with)\n";

    return 0;
}
