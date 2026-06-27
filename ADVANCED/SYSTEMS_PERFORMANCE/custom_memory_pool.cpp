// ADVANCED: Custom Memory Pool Allocator
//
// WHY THIS MATTERS:
// std::allocator (used by default in vector/map/etc.) calls new/delete for each
// element, which:
//   1. Has overhead: system call, bookkeeping, potential lock contention
//   2. Causes heap fragmentation over time
//   3. Has unpredictable latency (bad for real-time systems)
//
// A pool allocator pre-allocates a large block and sub-allocates from it:
//   - O(1) allocation and deallocation
//   - Zero fragmentation within the pool
//   - Cache-friendly (objects are contiguous)
//
// REAL-WORLD USE: game engines (Unreal uses this extensively), high-frequency
// trading (microsecond latency), embedded systems (no heap), network servers.

#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <cstring>
#include <memory>

// ============================================================
// Fixed-size block pool allocator
// All blocks in this pool are the same size.
// ============================================================
class FixedBlockPool {
private:
    size_t block_size;
    size_t capacity;
    char*  pool;           // raw memory
    char** free_list;      // pointer to next free block
    size_t allocated_count;

public:
    FixedBlockPool(size_t block_bytes, size_t num_blocks)
        : block_size(block_bytes), capacity(num_blocks), allocated_count(0)
    {
        // block_size must be at least sizeof(void*) to store free list pointer
        block_size = std::max(block_size, sizeof(char*));
        pool = new char[block_size * num_blocks];

        // Build free list: each block stores a pointer to the next free block
        free_list = reinterpret_cast<char**>(pool);
        char** current = free_list;
        for (size_t i = 0; i < num_blocks - 1; ++i) {
            *current = pool + (i + 1) * block_size;
            current = reinterpret_cast<char**>(*current);
        }
        *current = nullptr;  // last block points to null
    }

    ~FixedBlockPool() {
        delete[] pool;
    }

    // Non-copyable
    FixedBlockPool(const FixedBlockPool&) = delete;
    FixedBlockPool& operator=(const FixedBlockPool&) = delete;

    void* allocate() {
        if (!free_list) return nullptr;  // pool exhausted
        void* block = free_list;
        free_list = reinterpret_cast<char**>(*free_list);
        ++allocated_count;
        return block;
    }

    void deallocate(void* ptr) {
        char** block = reinterpret_cast<char**>(ptr);
        *block = reinterpret_cast<char*>(free_list);
        free_list = block;
        --allocated_count;
    }

    size_t get_allocated() const { return allocated_count; }
    size_t get_capacity()  const { return capacity; }

    void print_stats() const {
        std::cout << "Pool: block=" << block_size << " bytes"
                  << " cap=" << capacity
                  << " allocated=" << allocated_count
                  << " free=" << (capacity - allocated_count) << "\n";
    }
};

// ============================================================
// Object that uses the pool allocator
// ============================================================
struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float life;
    int   id;

    Particle(int id, float x, float y, float z)
        : x(x), y(y), z(z), vx(0), vy(0), vz(0), life(1.0f), id(id) {}

    void update(float dt) {
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;
        life -= dt * 0.1f;
    }
};

// ============================================================
// Benchmark: pool vs new/delete
// ============================================================
struct Timer {
    std::chrono::high_resolution_clock::time_point start;
    Timer() : start(std::chrono::high_resolution_clock::now()) {}
    double ms() const {
        return std::chrono::duration<double, std::milli>(
            std::chrono::high_resolution_clock::now() - start).count();
    }
};

int main() {
    std::cout << "=== Custom Memory Pool Allocator ===\n\n";

    const size_t N = 100000;  // number of allocations per test

    // --- Correctness test ---
    std::cout << "--- Correctness test ---\n";
    FixedBlockPool pool(sizeof(Particle), 10);
    pool.print_stats();

    Particle* p1 = new(pool.allocate()) Particle(1, 1.0f, 2.0f, 3.0f);
    Particle* p2 = new(pool.allocate()) Particle(2, 4.0f, 5.0f, 6.0f);
    pool.print_stats();

    std::cout << "p1: id=" << p1->id << " pos=(" << p1->x << "," << p1->y << "," << p1->z << ")\n";
    std::cout << "p2: id=" << p2->id << " pos=(" << p2->x << "," << p2->y << "," << p2->z << ")\n";

    p1->~Particle();
    pool.deallocate(p1);
    pool.print_stats();

    p2->~Particle();
    pool.deallocate(p2);
    pool.print_stats();

    // --- Benchmark: pool vs new/delete ---
    std::cout << "\n--- Benchmark: " << N << " allocations ---\n";

    FixedBlockPool bench_pool(sizeof(Particle), N);
    std::vector<void*> ptrs;
    ptrs.reserve(N);

    // Pool allocate
    {
        Timer t;
        for (size_t i = 0; i < N; ++i) {
            void* mem = bench_pool.allocate();
            new(mem) Particle(static_cast<int>(i), 0,0,0);
            ptrs.push_back(mem);
        }
        double alloc_ms = t.ms();

        Timer t2;
        for (void* p : ptrs) {
            static_cast<Particle*>(p)->~Particle();
            bench_pool.deallocate(p);
        }
        double free_ms = t2.ms();

        std::cout << "Pool allocator: alloc=" << alloc_ms << " ms"
                  << "  free=" << free_ms << " ms"
                  << "  total=" << alloc_ms + free_ms << " ms\n";
    }

    ptrs.clear();

    // Standard new/delete
    {
        Timer t;
        for (size_t i = 0; i < N; ++i) {
            ptrs.push_back(new Particle(static_cast<int>(i), 0,0,0));
        }
        double alloc_ms = t.ms();

        Timer t2;
        for (void* p : ptrs) delete static_cast<Particle*>(p);
        double free_ms = t2.ms();

        std::cout << "new/delete:     alloc=" << alloc_ms << " ms"
                  << "  free=" << free_ms << " ms"
                  << "  total=" << alloc_ms + free_ms << " ms\n";
    }

    std::cout << "\nConclusion:\n";
    std::cout << "  Pool allocator is faster because:\n";
    std::cout << "  - No system call per allocation\n";
    std::cout << "  - No heap bookkeeping\n";
    std::cout << "  - Objects are cache-contiguous\n";
    std::cout << "  - Deallocation is O(1) always\n";
    std::cout << "\nReal-world: game engines, trading systems, embedded RT systems\n";

    return 0;
}
