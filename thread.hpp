#ifndef THREAD_HPP
#define THREAD_HPP

#include <condition_variable>
#include <memory>
#include <sys/types.h>
#include <thread>
#include <chrono>
#include <iostream>

#include "random_num_gen.cpp"

extern int total_working;       // in main thread: if 5 threads are working, wait for at LEAST 1 to finish

class Thread {
    private: 
        uint16_t id;                                        // id of the thread
        std::unique_ptr<std::thread> _thread;               // the workerthread
        
        // method for the actual working thread
        static void workerThread(Thread* aThread, std::mutex& mutex, std::condition_variable& cv, int& ready);
    
    public:
        Thread(uint16_t id);                                // default constructor
        void doWork(std::mutex& mutex, std::condition_variable& cv, int& ready);   // simulates thread working
        uint16_t getID();                                   // returns id
        void run(std::mutex& mutex, std::condition_variable& cv, int& ready);      // creates the thread
        void wait();
};

#endif