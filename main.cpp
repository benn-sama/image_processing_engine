#include "thread_pool.hpp"

int main(int argc, char*argv[]) {
    int threadCount = 5;

    if (argc > 1) {
        threadCount = std::atoi(argv[1]);
    }

    ThreadPool pool(threadCount);
    pool.run();
}