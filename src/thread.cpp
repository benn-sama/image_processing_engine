#include "../include/thread.hpp"
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>

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
   Creates instance of thread and passes a function as the parameter and points to it
*/
void Thread::run(std::function<void()> func) {
  _thread = std::thread(std::move(func));
}

/*
 Method:
  wait()

 Desc:
  This method calls the main process to wait until this thread is terminated
*/
void Thread::wait() {
  if (_thread.joinable()) { _thread.join(); }
}

Thread::~Thread() {
  if (_thread.joinable()) { _thread.join(); }
}