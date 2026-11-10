#ifndef THREAD_HPP
#define THREAD_HPP

#include <condition_variable>
#include <memory>
#include <sys/types.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <stack>
#include <functional>

#include "../src/random_num_gen.cpp"

extern int total_working;       // in main thread: if n threads are working, wait for at LEAST 1 to finish

// class Thread {
//     private: 
//         uint16_t                     id;                    // id of the thread
//         std::unique_ptr<std::thread> _thread;               // the workerthread
        
//         // method for the actual working thread
//         static void workerThread(Thread* aThread, std::mutex& mutex, std::condition_variable& cv, int& ready);
    
//     public:
//         Thread(uint16_t id);                                                          // default constructor
//         void     doWork(std::mutex& mutex, std::condition_variable& cv, int& ready);  // simulates thread working
//         uint16_t getID();                                                             // returns id
//         void     run(std::mutex& mutex, std::condition_variable& cv, int& ready);     // creates the thread
//         void     wait();
// };

class Thread {
    private:
        int id;
        std::thread _thread;
        
    public:
        Thread(int id) : id(id) {} 

        // prevents/deletes the copy constructor
        Thread(Thread&&) noexcept            = default; // promises the caller it will not throw
        Thread& operator=(Thread&&) noexcept = default;
        Thread(const Thread&)                = delete;
        Thread& operator=(const Thread&)     = delete;

        void run(std::function<void()> func); // passes the function to work on and points towards it
        void wait(); // join the main thread
        ~Thread();

        u_int16_t getID();
};

#endif