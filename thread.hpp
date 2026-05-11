#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>
#include <chrono>
#include <iostream>

class WorkerThread {
    private: 
        uint16_t id;                     // id of the thread
        std::thread* _thread;            // the workerthread
    
    public:
        WorkerThread(uint16_t id);       // default constructor
        void doWork();                   // simulates thread working
        uint16_t getID();                // returns id
        void work(WorkerThread *thread); // the actual worker thread
        void run();                      // creates the thread
};

#endif