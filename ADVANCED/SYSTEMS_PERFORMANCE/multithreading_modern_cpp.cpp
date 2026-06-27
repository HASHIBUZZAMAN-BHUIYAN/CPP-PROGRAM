// ADVANCED: Multithreading with Modern C++ (std::thread, mutex, atomic)
//
// WHY THIS MATTERS:
// C++11 added threading to the standard library — no more platform-specific
// pthreads/Win32 threads. Modern C++ threading is portable, safe, and expressive.
//
// This covers the same concepts as the C-PROGRAM and JAVA-PROGRAM concurrency
// examples but using C++'s type-safe, RAII-based approach:
// - C (POSIX pthreads): pthread_create, pthread_mutex_lock — manual, error-prone
// - Java: synchronized methods, wait/notify, ExecutorService
// - C++ (this file): std::thread, std::mutex, std::atomic, std::condition_variable

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <queue>
#include <chrono>
#include <numeric>
#include <functional>
#include <future>

// ============================================================
// Utility
// ============================================================
void sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

std::mutex g_print_mutex;  // protect std::cout from concurrent writes

void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_print_mutex);
    std::cout << "[thread " << std::this_thread::get_id() << "] " << msg << "\n";
}

// ============================================================
// PART 1: std::thread basics
// ============================================================
void demo_thread_basics() {
    std::cout << "\n--- std::thread basics ---\n";

    // Launch a thread with a lambda
    std::thread t1([]{
        safe_print("Thread 1 running");
        sleep_ms(50);
        safe_print("Thread 1 done");
    });

    // Launch with a function
    auto worker = [](int id, int work_ms) {
        safe_print("Worker " + std::to_string(id) + " started");
        sleep_ms(work_ms);
        safe_print("Worker " + std::to_string(id) + " finished");
    };

    std::thread t2(worker, 2, 30);
    std::thread t3(worker, 3, 70);

    // Must join or detach before thread object is destroyed
    t1.join();
    t2.join();
    t3.join();
    std::cout << "All threads joined\n";
}

// ============================================================
// PART 2: Data race and mutex protection
// ============================================================
void demo_mutex() {
    std::cout << "\n--- Mutex / data race ---\n";

    int counter_unsafe = 0;
    std::atomic<int> counter_atomic{0};
    int counter_mutex = 0;
    std::mutex mtx;

    const int THREADS = 10, ITERATIONS = 10000;

    // Race condition (undefined behaviour — just for illustration)
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; ++i)
            threads.emplace_back([&]{ for(int j=0; j<ITERATIONS; ++j) ++counter_unsafe; });
        for (auto& t : threads) t.join();
        // Expected: THREADS * ITERATIONS = 100000, actual: LESS due to race
        std::cout << "Unsafe  counter: " << counter_unsafe
                  << " (expected " << THREADS * ITERATIONS << ", lost updates due to race)\n";
    }

    // std::atomic — lock-free, correct, but limited to simple types
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; ++i)
            threads.emplace_back([&]{ for(int j=0; j<ITERATIONS; ++j) ++counter_atomic; });
        for (auto& t : threads) t.join();
        std::cout << "Atomic  counter: " << counter_atomic.load()
                  << " (exactly correct)\n";
    }

    // std::mutex — correct for any data
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; ++i)
            threads.emplace_back([&]{
                for (int j = 0; j < ITERATIONS; ++j) {
                    std::lock_guard<std::mutex> lock(mtx);  // RAII: auto-unlocks
                    ++counter_mutex;
                }
            });
        for (auto& t : threads) t.join();
        std::cout << "Mutex   counter: " << counter_mutex
                  << " (exactly correct, more overhead than atomic)\n";
    }
}

// ============================================================
// PART 3: Producer-Consumer with condition_variable
// ============================================================
template<typename T>
class ThreadSafeQueue {
    std::queue<T>           q;
    mutable std::mutex      mtx;
    std::condition_variable cv;
    bool                    done{false};

public:
    void push(T val) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(std::move(val));
        }
        cv.notify_one();
    }

    bool pop(T& val) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !q.empty() || done; });
        if (q.empty()) return false;
        val = std::move(q.front());
        q.pop();
        return true;
    }

    void signal_done() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            done = true;
        }
        cv.notify_all();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.size();
    }
};

void demo_producer_consumer() {
    std::cout << "\n--- Producer-Consumer ---\n";

    ThreadSafeQueue<int> queue;
    std::atomic<int> total_consumed{0};

    // Producer
    auto producer = [&](int id, int count) {
        for (int i = 0; i < count; ++i) {
            queue.push(i * 10 + id);
            sleep_ms(5);
        }
        safe_print("Producer " + std::to_string(id) + " done");
    };

    // Consumer
    auto consumer = [&](int id) {
        int val;
        int count = 0;
        while (queue.pop(val)) {
            ++count;
            ++total_consumed;
        }
        safe_print("Consumer " + std::to_string(id) + " processed " + std::to_string(count));
    };

    std::thread p1(producer, 1, 5);
    std::thread p2(producer, 2, 5);
    std::thread c1(consumer, 1);
    std::thread c2(consumer, 2);

    p1.join();
    p2.join();
    queue.signal_done();
    c1.join();
    c2.join();

    std::cout << "Total consumed: " << total_consumed.load() << " (expected 10)\n";
}

// ============================================================
// PART 4: std::async and std::future — high-level parallelism
// ============================================================
void demo_future_async() {
    std::cout << "\n--- std::async / std::future ---\n";

    // Parallel computation without manual thread management
    auto heavy_work = [](int n, const std::string& name) -> long long {
        safe_print(name + " started");
        long long sum = 0;
        for (int i = 1; i <= n; ++i) sum += i;
        sleep_ms(100);
        safe_print(name + " done = " + std::to_string(sum));
        return sum;
    };

    // Launch tasks asynchronously — run in parallel
    auto f1 = std::async(std::launch::async, heavy_work, 1000000, "Task A");
    auto f2 = std::async(std::launch::async, heavy_work, 2000000, "Task B");
    auto f3 = std::async(std::launch::async, heavy_work, 500000,  "Task C");

    // .get() blocks until the result is ready
    long long r1 = f1.get();
    long long r2 = f2.get();
    long long r3 = f3.get();

    std::cout << "Results: A=" << r1 << " B=" << r2 << " C=" << r3 << "\n";
    std::cout << "Total: " << (r1 + r2 + r3) << "\n";
}

int main() {
    std::cout << "=== Modern C++ Multithreading ===\n";
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << " threads\n";

    demo_thread_basics();
    demo_mutex();
    demo_producer_consumer();
    demo_future_async();

    std::cout << "\n=== Cross-Language Comparison ===\n";
    std::cout << "C (pthreads):    pthread_create, pthread_mutex_lock — manual, no RAII\n";
    std::cout << "Java:            synchronized, wait/notify, ExecutorService\n";
    std::cout << "C++ (this file): std::thread, std::mutex (RAII), std::atomic, std::async\n";
    std::cout << "\nC++ advantages:\n";
    std::cout << "  - lock_guard/unique_lock: RAII-based, auto-unlock even on exception\n";
    std::cout << "  - std::atomic: lock-free operations for simple types\n";
    std::cout << "  - std::async: high-level futures without manual thread management\n";
    std::cout << "  - move semantics: pass work to threads efficiently\n";

    return 0;
}
