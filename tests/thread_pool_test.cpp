#include "../include/image_processor.hpp"
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include "thread_pool.hpp"
#include <chrono>
#include <mutex>
#include <queue>
#include <stack>
#include <thread>

// returns top of stack and removes top value
template <typename  T>
T draw(std::stack<T>& s) {
    T value = s.top();
    s.pop();
    return value;
}

// initializies the size of the stack
void init_stack(std::stack<int>& stack, int size) {
    for (int i = 0; i < size; ++i) {
        stack.push(i);
    }
}

// prints the top of the stack
void draw_stack(std::stack<int>& stack) {
    static std::mutex mtx;
    
    std::lock_guard<std::mutex> lck(mtx);
    int num = ::draw(stack);
}


TEST_CASE("Testing Thread Pool", "[thread]") {
    std::stack<int> stack_of_nums;
    init_stack(stack_of_nums, 1000);

    ThreadPool pool(5);

    int expected = 0;
    int actual   = 0;

    expected = 1000;
    actual   = stack_of_nums.size(); 
    
    CAPTURE(expected, actual);
    REQUIRE(expected == actual);

    // *************** TEST IF STACK IS EMPTY AFTER FILLING QUEUE ********************

    for (int i = 0; i < 1000; ++i) {
        pool.add_queue([&stack_of_nums] { draw_stack(stack_of_nums); });
    }

    expected = 0;
    while (actual != 0) {
        actual = stack_of_nums.size();
    }

    CAPTURE(expected, actual);
    REQUIRE(expected == actual);

    pool.shut_off_pool();
}