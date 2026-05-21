#include "thread_pool.hpp"
#include "image_processor.hpp"

int main(int argc, char*argv[]) {
    int threadCount = 5;

    // if (argc > 1) {
    //     threadCount = std::atoi(argv[1]);
    // }

    // ThreadPool pool(threadCount);
    // pool.run();

    std::string dir    = "";
    char*       choice = nullptr;

    if (argc > 1) {
        dir    = std::string(argv[1]);
        choice = argv[2];
    } else {
        std::cout << "No arguments provided\n";
        std::cout << "./main <photo-dir> <choice>\n";
        return 1;
    }

    // ThreadPool pool(threadCount);
    // pool.run();
    
    Image img;
    img.source(dir);
    img.greyscale(*choice);
}