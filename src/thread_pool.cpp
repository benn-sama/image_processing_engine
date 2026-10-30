#include "../include/thread_pool.hpp"
#include "../include/thread.hpp"
#include <memory>
#include <mutex>

int total_working = 0;

ThreadPool::ThreadPool(short threadCount)
    : THREAD_COUNT(threadCount) {

    std::cout << "Intializing threadpool." << std::endl
              << "Thread Count: " << threadCount << std::endl;

    for (int i = 0; i < THREAD_COUNT; ++i) {
        // pool.push_back(new Thread(i));
        pool.push_back(std::make_unique<Thread>(i));
    }
    
    std::cout << "Threadpool initialized.\n";
    std::cout << "Starting threads.\n";

    this->startThreads();
}

void ThreadPool::startThreads() {
    std::cout << "Starting up threads.\n";

    for (const auto& thread : pool) {
        thread->run(mutex, cv, ready);
    }

    std::cout << "All threads have started.\n";
}

void ThreadPool::run() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lck(mutex);
            cv.wait(lck, [this] { return shut_off || !work_queue.size(); });
            if (shut_off) { return; }
            task = pop_front(work_queue);
        }
        task(); // work the task
    }
}

// void ThreadPool::run() {
//     this->startThreads();

//     while (true) {
//         while (correctNumber()) {
//             std::unique_lock<std::mutex> lk(mutex);

//             if (total_working < 5) {
//                 ++count;
//                 ++total_working;
//                 std::cout << count << ": Signaling thread.\n";

//                 ready = 1; // this gives the thread a green light to go ahead
//                 lk.unlock();
                
//                 cv.notify_one(); // this actually signals to the thread
//                 std::cout << count << ": Thread signaled\n";
//             }
//         }
//     }
// }

/*
What this does:
  - Fills the work_queue with jobs

What are the jobs?
  These jobs are ints that represent each row up to the row_size of the photo
*/
void ThreadPool::add_queue(std::function<void ()> func) {
    work_queue.push(func);
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
    cv.notify_all(); // wakey wakey all threads, time for school
    for (auto& thread : pool) {
        thread.reset();
    }
}