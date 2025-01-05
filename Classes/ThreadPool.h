//------------------------------------Refactored with Thread Pool Pattern--------------------------------------//
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "cocos2d.h"

// Create the ThreadPool class
// The ThreadPool class is responsible for managing multiple worker threads and the task queue
class ThreadPool {
public:
    // Constructor to initialize the thread pool
    ThreadPool(size_t numThreads);

    // Destructor to destroy the thread pool
    ~ThreadPool();

    // Submit a task to the thread pool
    void enqueueTask(std::function<void()> task);

private:
    // Container to store the worker threads
    std::vector<std::thread> workers;

    // Task queue to store the tasks to be processed
    std::queue<std::function<void()>> tasks;

    // Mutex to ensure thread safety when accessing the task queue in a multithreaded environment
    std::mutex queueMutex;

    // Condition variable for thread synchronization
    std::condition_variable condition;

    // Flag to control when to stop the thread pool
    bool stop;

    // Worker thread function that each thread executes
    void workerThread();
};

#endif // THREAD_POOL_H