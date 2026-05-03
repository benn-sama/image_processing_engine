#include "thread.hpp"

WorkerThread::WorkerThread(uint16_t id) {
    this->id = id;
}

void WorkerThread::doWork() {
    using namespace std::literals::chrono_literals;
    std::cout << id << ": working...\n";
    std::this_thread::sleep_for(5s);
    std::cout << id << ": done!\n";
}