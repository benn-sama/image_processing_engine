#include "thread.hpp"

#include <cstdlib>
#include <semaphore>
#include <stdexcept>
#include <vector>
#include <random>
#include <mutex>

// returns a random number
int randomNum() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 10); // 1-10

    return dist(rng);
}

// 70% probablity of being right
bool correctNumber() {
    return random() <= 7;
}

int total_working = 0;

int main() {
    std::vector<Thread*> pool;
    const short THREAD_COUNT = 10;
    int count = 0;


    std::mutex mutex;            // for locking
    std::condition_variable cv;  // for signaling
    int ready = 0;               // conditon check just in case

    // create thread pool
    std::cout << "Intializing a threadpool\n";
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pool.push_back(new Thread(i));
    }
    std::cout << "Threadpool initialized.\n";

    // start up thread
    std::cout << "Starting threads\n";
    for (const auto& thread : pool) {
        thread->run(mutex, cv, ready);
    }
    std::cout << "All threads have started.\n";

    while (true) {
        // allows a thread to run
        while (correctNumber()) {
            std::unique_lock<std::mutex> lk(mutex);
            if (total_working < 5) {
                ++count; 
                std::cout << count << ": Signaling Thread" << std::endl;

                ready = 1;
                lk.unlock();

                cv.notify_one();
                std::cout << count << ": Thread signaled" << std::endl;
            } else {
                std::cout << "-----------Queue is full. Waiting for an open spot.-----------";
            }
        }
    }
}