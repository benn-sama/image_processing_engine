#include <chrono>
#include <thread> 
#include <chrono>
#include <iostream>

#include "../filter.hpp"

using namespace std::chrono;

steady_clock::time_point start_clock() {
    return std::chrono::steady_clock::now();
}

steady_clock::time_point end_clock() {
    return std::chrono::steady_clock::now();
}

auto elapsed_time(steady_clock::time_point start, steady_clock::time_point end) {
    return duration_cast<milliseconds>(end - start);
}

int main() {
    auto start = start_clock();

    std::this_thread::sleep_for(seconds(2));

    auto end = end_clock();
    std::cout << "Elapsed time: " << elapsed_time(start, end) << std::endl;
}