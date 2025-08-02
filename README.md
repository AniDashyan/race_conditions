# Race Conditions

## Overview

This project demonstrates  **race conditions** in C++. It compares three different approaches to incrementing a shared counter across multiple threads:

1. **Unsafe Counter** - No synchronization (causes race conditions)
2. **Atomic Counter** - Lock-free atomic operations 
3. **Mutex Counter** - Mutex-based synchronization

## Build & Run

```bash
# Clone the repository
git clone https://github.com/AniDashyan/race_conditions
cd race_conditions

# Build the project
cmake -S . -B build
cmake --build build

# Run the executable
./build/data_race
```

## Example Output

```
=== Thread Synchronization Demonstration ===
Expected result: 1000000
Number of threads: 100
Increments per thread: 10000

1. UNSAFE COUNTER (Race Condition):
Execution time: 218 ms
Final value: 13031
Difference from expected: 986969
Data race detected: YES

2. ATOMIC COUNTER (Thread-safe):
Execution time: 22 ms
Final value: 1000000
Difference from expected: 0
Correct result: YES

3. MUTEX COUNTER (Thread-safe):
Execution time: 82 ms
Final value: 1000000
Difference from expected: 0
Correct result: YES
```

## How Does It Work?

### Race Condition Demonstration

The **unsafe counter** deliberately creates a race condition by separating the read-modify-write operation:

```cpp
int temp = unsafe_counter;     // Read
temp = temp + 1;               // Modify  
std::this_thread::yield();     // Give other threads a chance
unsafe_counter = temp;         // Write
```

**What happens:** Multiple threads can read the same value, increment it, and write back the same result, causing **lost updates**.

### Thread-Safe Solutions

**Atomic Operations:**
```cpp
atomic_counter++;  // Hardware-level atomic increment
```
- Lock-free, fast performance
- Guarantees atomicity at hardware level

**Mutex Protection:**
```cpp
std::lock_guard<std::mutex> lock(counter_mutex);
mutex_counter++;  // Protected by mutex
```
- Ensures mutual exclusion
- Slower due to OS-level locking

### Performance Characteristics

- **Unsafe**: Fastest execution, incorrect results
- **Atomic**: ~2-3x slower than unsafe, correct results  
- **Mutex**: ~10-50x slower than atomic, correct results
