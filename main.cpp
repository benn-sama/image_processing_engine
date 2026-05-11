#include "thread.hpp"

#include <vector>
#include <semaphore>

int main() {
    std::vector<WorkerThread*> pool;

    // create thread pool
    for (int i = 0; i < 5; ++i) {
        pool.push_back(new WorkerThread(i));
    }

    // start up thread
    for (const auto& thread : pool) {
        thread->run();
    }
}