#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "thread.hpp"

#include <condition_variable>
#include <mutex>
#include <vector>

class ThreadPool{
    private:
        std::vector<Thread*>    pool;
        const short             THREAD_COUNT;      // total thread count
        int                     count = 0;         // total times a thread has been signaled
        std::mutex              mutex;             // protect resources
        std::condition_variable cv;                // for signaling
        int                     ready = 0;         // prevent spurious wakeups

        std::vector <int>       queue;             // task queue
        void                    startThreads();                       // starts the threads
    public:
        ThreadPool(short threadCount);             // constructor that initializes the number of threads
        void run();                                // threads start running here
};

#endif