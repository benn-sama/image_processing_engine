#include "thread.hpp"

/*
 Method: 
   Constructor
 
 Desc:
   This constructor assings an id to this instance
*/


WorkerThread::WorkerThread(uint16_t id) {
    this->id = id;
}

/*
 Method: 
   doWork()

 Desc:
   This method allows the thread to simulate "work"
*/

void WorkerThread::doWork() {
    using namespace std::literals::chrono_literals;
    std::cout << id << ": wokeup\n";
    std::cout << id << ": working...\n";
    std::this_thread::sleep_for(5s);
    std::cout << id << ": done!\n";
    std::cout << id << ": going back to sleep.\n";
}

/*
 Method: 
   getID()

 Desc:
   Returns this objects id
*/

uint16_t WorkerThread::getID() {
    return id;
}

/*
 Method: 
   work(WorkerThread *thread)

 Desc:
   This isntance allows the thread to simulate work
*/

void WorkerThread::work(WorkerThread *thread) {
    while(true) 
    {
        this->doWork();
    }
}

/*
 Method:
   run()

 Desc:
   Creates thread and pass this instance as the parameter
*/

void WorkerThread::run() {
    _thread = new std::thread(work, this);
}