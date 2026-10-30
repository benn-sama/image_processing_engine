#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "image_processor.hpp"
#include "thread.hpp"

#include <functional>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <atomic>
#include <queue>

template <typename T>
T pop_front(std::queue<std::function<T>> func) {
    T value = std::move(func.front());
    func.pop();
    return value;
}

class ThreadPool{
    private:
        std::vector<std::unique_ptr<Thread>>    pool;
        const short             THREAD_COUNT;      // total thread count
        int                     count = 0;         // total times a thread has been signaled
        std::mutex              mutex;             // protect resources
        std::condition_variable cv;                // for signaling
        int                     ready = 0;         // prevent spurious wakeups
        bool                    shut_off = 0;

        // this will definitely be the shared resource
        std::queue<std::function<void()>> work_queue; // task queue 
        void                     startThreads();      // starts the threads
        std::atomic<bool>        is_work_available = false;
    public:
        ThreadPool(short threadCount);             // constructor that initializes the number of threads
        void add_queue(std::function<void()> func);
        void run();                                // threads start running here
        void shut_off_pool();
        int clear_work_queue();
        int get_work_count() { return work_queue.size(); }
};

#endif