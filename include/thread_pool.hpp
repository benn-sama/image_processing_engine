#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "thread.hpp"

#include <functional>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <atomic>
#include <queue>

template <typename T>
auto pop_front(T& q) {
    auto value = std::move(q.front());
    q.pop();
    return value;
}

class ThreadPool{
    private:
        std::vector<std::unique_ptr<Thread>>    pool;
        short                   THREAD_COUNT = 5;      // total thread count
        int                     count = 0;         // total times a thread has been signaled
        std::mutex              mutex;             // protect resources
        std::condition_variable cv;                // for signaling
        int                     ready = 0;         // prevent spurious wakeups
        bool                    shut_off = 0;

        // this will definitely be the shared resource
        std::queue<std::function<void()>> work_queue; // task queue 
    public:
        ThreadPool(short threadCount);             // constructor that initializes the number of threads
        void add_queue(std::function<void()> func);
        void shut_off_pool();
        int  clear_work_queue();
        int  get_work_count() { return work_queue.size(); }
        void workerLoop(int id);
        ~ThreadPool();
};

#endif