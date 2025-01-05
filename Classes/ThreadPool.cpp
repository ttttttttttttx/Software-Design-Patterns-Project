//------------------------------------Refactored with Thread Pool Pattern--------------------------------------//
#include "ThreadPool.h"
// Implementing the ThreadPool class
// Constructor: Initializes the thread pool and creates the specified number of worker threads
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    // Create and start threads
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);  // Each thread calls the workerThread function
    }
}

// Destructor: Destroys the thread pool and stops all threads
ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;  // Set the stop flag to notify all threads to exit
    }
    condition.notify_all();  // Notify all threads to exit the loop
    for (std::thread& worker : workers) {
        worker.join();  // Wait for each thread to finish
    }
}

// Submit a task to the thread pool: Add the task to the task queue
void ThreadPool::enqueueTask(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);  // Lock to ensure thread safety
        tasks.push(task);  // Add the task to the task queue
    }
    condition.notify_one();  // Wake up one idle worker thread to execute the task
}

// Each worker thread executes this function
void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            // Wait and get a task
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });  // Wait for the task queue to be non-empty or the stop flag
            if (stop && tasks.empty()) {
                return;  // If the thread pool is stopping and the task queue is empty, exit the thread
            }
            task = tasks.front();  // Get the first task from the queue
            tasks.pop();  // Remove the task from the queue
        }
        task();  // Execute the task
    }
}
