#include "thread.hpp"

#include <cstdlib>
#include <semaphore>
#include <vector>
#include <random>

# define SLOTS 5 // num of allowed thread to work concurrently

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

    // create thread pool
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pool.push_back(new Thread(i));
    }

    // start up thread
    for (const auto& thread : pool) {
        thread->run(sem);
    }

    // wait for all the threads
    for (const auto& thread: pool) {
        thread->wait();
    }

    // allows a thread to run
    while (correctNumber()) {
        sem.release();
    }
}