#include "../include/image_processor.hpp"
#include "catch2/catch_test_macros.hpp"
#include "thread_pool.hpp"
#include <queue>
#include <stack>

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

// returns the top of the stack
int draw_stack(std::stack<int>& stack) {
    return ::draw(stack);
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

    pool.shut_off_pool();
}