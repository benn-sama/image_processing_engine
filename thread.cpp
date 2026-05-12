#include "thread.hpp"

/*
 Method: 
   Constructor
 
 Desc:
   This constructor assings an id to this instance
*/


Thread::Thread(uint16_t id) {
    this->id = id;
}

/*
 Method: 
   doWork()

 Desc:
   This method allows the thread to simulate "work"
*/

void Thread::doWork(auto& sem) {
    using namespace std::literals::chrono_literals;

    while (true) {
      // -- SEMAPHORE: acquires here, waits until signaled
      sem.acquire();

      std::cout << id << ": wokeup\n";
      std::cout << id << ": working...\n";
      std::this_thread::sleep_for(5s);
      std::cout << id << ": done!\n";
      std::cout << id << ": going back to sleep.\n";
      
      // -- SEMAPHORE: release lock here --
      sem.release();

    }
}

/*
 Method: 
   getID()

 Desc:
   Returns this objects id
*/
uint16_t Thread::getID() {
    return id;
}

/*
 Method:
   run()

 Desc:
   Creates instance of thread and pass this instance as the parameter
*/
void Thread::run(auto& sem) {
    _thread = new std::thread(workerThread, this, sem);
}

/*
 Method: 
   workerThread(Thread* thread)

 Desc:
   This calls the do work function
*/
void Thread::workerThread(Thread* aThread, auto& sem) {
  aThread->doWork(sem);
}

/*
 Method:
  wait()

 Desc:
  This method calls the main process to wait until this thread is terminated
*/
void Thread::wait() {
  if (_thread != NULL) {
    _thread->join();
  }
}