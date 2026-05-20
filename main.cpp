#include "thread_pool.hpp"
#include "image_processor.hpp"

int main(int argc, char*argv[]) {
    int threadCount = 5;

    // if (argc > 1) {
    //     threadCount = std::atoi(argv[1]);
    // }

    // ThreadPool pool(threadCount);
    // pool.run();
    
    Image img;
    std::string dir = "photos/hills.ppm";
    img.source(dir);
    char choice = 'l';
    img.greyscale(choice);
}