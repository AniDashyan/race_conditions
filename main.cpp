#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>

// Global variables for demonstration
int unsafe_counter = 0;
std::atomic<int> atomic_counter{0};
int mutex_counter = 0;
std::mutex counter_mutex;

const int NUM_THREADS = 100;          
const int INCREMENTS_PER_THREAD = 10000;

// 1. FAULTY CODE: Race condition with unsafe counter
void unsafe_increment() {
    for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
        int temp = unsafe_counter;
        std::this_thread::yield(); 
        unsafe_counter = temp + 1;
    }
}


// 2. FIXED VERSION: Using atomic operations
void atomic_increment() {
    for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
        atomic_counter++;
    }
}

// 3. MUTEX-BASED SYNCHRONIZATION
void mutex_increment() {
    for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
        std::lock_guard<std::mutex> lock(counter_mutex);
        mutex_counter++;  // Protected by mutex
    }
}

// Performance timing function
template<typename Func>
double time_execution(Func func, const std::string& description) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << description << ": " << duration.count() << " ms" << std::endl;
    
    return duration.count();
}

// Universal templated test function
template<typename WorkerFunc>
void run_threaded_test(WorkerFunc func) {
    std::vector<std::thread> threads;
    
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(func);
    }
    
    // Wait for all threads to complete
    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    const int expected_result = NUM_THREADS * INCREMENTS_PER_THREAD;
    
    std::cout << "=== Thread Synchronization Demonstration ===" << std::endl;
    std::cout << "Expected result: " << expected_result << std::endl;
    std::cout << "Number of threads: " << NUM_THREADS << std::endl;
    std::cout << "Increments per thread: " << INCREMENTS_PER_THREAD << std::endl << std::endl;
    
    // Test 1: Unsafe counter (race condition)
    std::cout << "1. UNSAFE COUNTER (Race Condition):" << std::endl;
    unsafe_counter = 0;  // Reset counter
    time_execution([](){ run_threaded_test(unsafe_increment); }, "Execution time");
    std::cout << "Final value: " << unsafe_counter << std::endl;
    std::cout << "Difference from expected: " << (expected_result - unsafe_counter) << std::endl;
    std::cout << "Data race detected: " << (unsafe_counter != expected_result ? "YES" : "NO") << std::endl << std::endl;
    
    // Test 2: Atomic counter
    std::cout << "2. ATOMIC COUNTER (Thread-safe):" << std::endl;
    atomic_counter = 0;  // Reset counter
    time_execution([](){ run_threaded_test(atomic_increment); }, "Execution time");
    std::cout << "Final value: " << atomic_counter.load() << std::endl;
    std::cout << "Difference from expected: " << (expected_result - atomic_counter.load()) << std::endl;
    std::cout << "Correct result: " << (atomic_counter.load() == expected_result ? "YES" : "NO") << std::endl << std::endl;
    
    // Test 3: Mutex counter
    std::cout << "3. MUTEX COUNTER (Thread-safe):" << std::endl;
    mutex_counter = 0;  // Reset counter
    time_execution([](){ run_threaded_test(mutex_increment); }, "Execution time");
    std::cout << "Final value: " << mutex_counter << std::endl;
    std::cout << "Difference from expected: " << (expected_result - mutex_counter) << std::endl;
    std::cout << "Correct result: " << (mutex_counter == expected_result ? "YES" : "NO") << std::endl << std::endl;
    
    return 0;
}
