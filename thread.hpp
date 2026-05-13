#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>
#include <chrono>
#include <iostream>
#include <semaphore>

class Thread {
    private: 
        uint16_t id;                                // id of the thread
        std::thread* _thread;                       // the workerthread
        static void workerThread(Thread* aThread, auto& sem);  // method for the actual working thread
    
    public:
        Thread(uint16_t id);                        // default constructor
        void doWork(auto& sem);                     // simulates thread working
        uint16_t getID();                           // returns id
        void run(auto& sem);                        // creates the thread
        void wait();
};

#endif