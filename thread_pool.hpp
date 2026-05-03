#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <thread>

class ThreadPool{
    public:
        ThreadPool();  // constructor
        void wakeUp(); // notifies one thread to wake up and do work
}

#endif