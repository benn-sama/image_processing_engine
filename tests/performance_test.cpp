#include <chrono>
#include <iostream>

#include "../image_processor.hpp"

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
    char lumin = 'l';
    char avg = 'a';
    char lightness = 't';
    std::string src1 = "./photos/4k_1.ppm";
    std::string src2 = "./photos/4k_2.ppm";
    std::string src3 = "./photos/1080_1.ppm";
     
    auto start = start_clock();

    Image high_res_img_1(src1);
    high_res_img_1.greyscale(lumin);

    auto end = end_clock();
    std::cout << "4k_1 time elapsed: " << elapsed_time(start, end) << std::endl;

    // ------------------------------------------------------------------------------------

    start = start_clock();

    Image high_res_img_2(src2);
    high_res_img_1.greyscale(lumin);

    end = end_clock();
    std::cout << "4k_2 time elapsed: " << elapsed_time(start, end) << std::endl;
    
    // ------------------------------------------------------------------------------------

    start = start_clock();

    Image med_res_img_1(src3);
    high_res_img_1.greyscale(lumin);

    end = end_clock();
    std::cout << "1080_1 time elapsed: " << elapsed_time(start, end) << std::endl;
}