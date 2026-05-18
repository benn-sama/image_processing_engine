#include "thread_pool.hpp"
#include "thread.hpp"

int total_working = 0;

ThreadPool::ThreadPool(short threadCount)
    : THREAD_COUNT(threadCount) {

    std::cout << "Intializing threadpool." << std::endl
              << "Thread Count: " << threadCount << std::endl;

    for (int i = 0; i < THREAD_COUNT; ++i) {
        pool.push_back(new Thread(i));
    }
    
    std::cout << "Threadpool initialized.\n";
}

void ThreadPool::startThreads() {
    std::cout << "Starting up threads.\n";

    for (const auto& thread : pool) {
        thread->run(mutex, cv, ready);
    }

    std::cout << "All threads have started.\n";
}

void ThreadPool::run() {
    this->startThreads();

    while (true) {
        while (correctNumber()) {
            std::unique_lock<std::mutex> lk(mutex);

            if (total_working < 5) {
                ++count;
                ++total_working;
                std::cout << count << ": Signaling thread.\n";

                ready = 1; // this gives the thread a green light to go ahead
                lk.unlock();
                
                cv.notify_one(); // this actually signals to the thread
                std::cout << count << ": Thread signaled\n";
            }
        }
    }
}