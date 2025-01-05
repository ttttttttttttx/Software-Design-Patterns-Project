//------------------------------------Refactored with Thread Pool Pattern--------------------------------------//
#include "ThreadPool.h"

// Constructor initializes the thread pool with the specified number of threads
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    // Create and start worker threads
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

// Destructor ensures proper cleanup of all threads
ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;    // Signal all threads to stop
    }
    condition.notify_all();  // Wake up all threads
    // Wait for all threads to finish
    for (std::thread& worker : workers) {
        worker.join();
    }
}

// Add a new task to the thread pool queue
void ThreadPool::enqueueTask(std::shared_ptr<Task> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push(task);    // Add task to queue
    }
    condition.notify_one();  // Wake up one worker thread
}

// Main worker thread function
void ThreadPool::workerThread() {
    while (true) {
        std::shared_ptr<Task> task;
        {
            // Wait for a task or stop signal
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            
            // Exit if thread pool is stopping and no tasks remain
            if (stop && tasks.empty()) {
                return;
            }
            
            // Get next task from queue
            task = tasks.front();
            tasks.pop();
        }
        
        // Execute the task if valid
        if (task) {
            task->execute();
        }
    }
}
