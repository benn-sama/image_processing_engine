#include "thread.hpp"
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>

int randomTime() {
    RandomNum random;
    return random.getRandomNum(1000, 5000);
}

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

void Thread::doWork(std::mutex& mutex, std::condition_variable& cv, int& ready) {
    using namespace std::literals::chrono_literals;
    RandomNum random;

    while (true) {
      {
      std::unique_lock<std::mutex> lk(mutex);
      std::cout << id << ": Hello!" << std::endl;
      // -- THREAD WAITS HERE
      cv.wait(lk, [&ready] { return ready > 0; });

      std::cout << id << ": wokeup\n";
      std::cout << id << ": working...\n";
      
      // reset ready
      ready = 0;
      }
      
      std::this_thread::sleep_for(std::chrono::milliseconds(random.getRandomNum(1000, 10000)));
      std::cout << id << ": done!\n";
      std::cout << id << ": going back to sleep.\n";
    
      {
        std::unique_lock<std::mutex> lk(mutex);
        --total_working;
      }

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
void Thread::run(std::mutex& mutex, std::condition_variable& cv, int& ready) {
    // _thread = new std::thread(workerThread, this, std::ref(sem));
    _thread = std::make_unique<std::thread>(
      &Thread::workerThread, 
      this, 
      std::ref(mutex), 
      std::ref(cv), 
      std::ref(ready));
}

/*
 Method: 
   workerThread(Thread* thread)

 Desc:
   This calls the do work function
*/
void Thread::workerThread(Thread* aThread, std::mutex& mutex, std::condition_variable& cv, int& ready) {
  aThread->doWork(mutex, cv, ready);
}

/*
 Method:
  wait()

 Desc:
  This method calls the main process to wait until this thread is terminated
*/
void Thread::wait() {
  if (_thread && _thread->joinable()) {
    _thread->join();
  }
}