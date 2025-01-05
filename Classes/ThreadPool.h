//------------------------------------Refactored with Thread Pool Pattern--------------------------------------//
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>
#include "Task.h"

// ThreadPool class manages a pool of worker threads for executing tasks
class ThreadPool {
public:
    // Constructor creates the thread pool with specified number of threads
    ThreadPool(size_t numThreads);
    // Destructor ensures proper cleanup of threads
    ~ThreadPool();

    // Submit a task to be executed by the thread pool
    void enqueueTask(std::shared_ptr<Task> task);

private:
    // Container for worker threads
    std::vector<std::thread> workers;
    // Queue of tasks waiting to be executed
    std::queue<std::shared_ptr<Task>> tasks;
    // Mutex for thread-safe access to the task queue
    std::mutex queueMutex;
    // Condition variable for thread synchronization
    std::condition_variable condition;
    // Flag to signal threads to stop
    bool stop;

    // Worker thread function that processes tasks
    void workerThread();
};

#endif // THREAD_POOL_H