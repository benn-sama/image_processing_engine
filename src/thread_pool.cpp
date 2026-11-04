#include "../include/thread_pool.hpp"
#include "../include/thread.hpp"
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

int total_working = 0;

ThreadPool::ThreadPool(short threadCount)
    : THREAD_COUNT(threadCount) {

    for (int i = 0; i < THREAD_COUNT; ++i) {
        pool.emplace_back(std::make_unique<Thread>(i));
        pool.back()->run([this, i] { workerLoop(i); }); // lambda func that calls the worker loop
    }

}

void ThreadPool::workerLoop(int i) {
    for (;;) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lck(mutex);
            cv.wait(lck, [this] { return shut_off || !work_queue.empty(); });
            if (shut_off && work_queue.empty()) { return; }
            task = std::move(::pop_front(work_queue));
        }
        task();
    }

    
}

void ThreadPool::add_queue(std::function<void()> func) {
    {
        std::lock_guard<std::mutex> lck(mutex);
        work_queue.push(func);
    }
    cv.notify_one();
}

/*
What this does:
  - Completely clear the work_queue
*/
int ThreadPool::clear_work_queue() {
    if (int size = (int)work_queue.size() > 0) {
        for (int i = 0; i < size; ++i) {
            work_queue.pop();
        }
    }

    return work_queue.size() > 0 ? 0 : 1;
}

/*
What this does:
  - Wakes up all threads
  - Deallocates, then calls join
*/
void ThreadPool::shut_off_pool() {
    {
        std::lock_guard<std::mutex> lck(mutex);
        shut_off = true;
    }

    cv.notify_all(); // wakey wakey all threads, time for school
    for (auto& thread : pool) {
        thread->wait();
        thread.reset();
    }
}