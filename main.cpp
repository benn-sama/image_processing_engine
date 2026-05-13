#include "thread.hpp"

#include <cstdlib>
#include <semaphore>
#include <stdexcept>
#include <vector>
#include <random>

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

std::counting_semaphore<SLOTS> sem(0); // initialized only one

int main() {
    std::vector<Thread*> pool;
    const short THREAD_COUNT = 5;
    int count = 0;

    try {
        std::cout << "Intializing a threadpool\n";
        // create thread pool
        for (int i = 0; i < THREAD_COUNT; ++i) {
            pool.push_back(new Thread(i));
        }
        std::cout << "Threadpool initialized.\n";

        std::cout << "Starting threads\n";
        // start up thread
        for (const auto& thread : pool) {
            thread->run(sem);
        }
        std::cout << "All threads have started.\n";

        // std::cout << "Joining threads\n";
        // // wait for all the threads
        // for (const auto& thread: pool) {
        //     thread->wait();
        // }
        // std::cout << "Threads have joined.\n";
    } catch (std::invalid_argument& e) {
        std::cout << "ERROR: DURING THREAD START\n" << e.what() << std::endl;
        return 1;
    }

    try {
        while (true) {
            // allows a thread to run
            while (correctNumber()) {
                ++count;
                std::cout << count << " : Thread signaled" << std::endl;
                sem.release();
            }
        }
    } catch (std::invalid_argument& e) {
        std::cout << "ERROR: DURING THREAD WORK\n" << e.what() << std::endl;
    }
}