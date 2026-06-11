#include "scanline.hpp"
#include "image_processor.hpp"
#include <memory>
#include <unistd.h>

int main() {
    Scanline sn;
    Image    img;

    std::string str = "./photos/hills.ppm"; 
    img.source(str);
    std::fstream* src = img.get_ptr();

    std::cout << "running...\n";
    std::cout << "allocating...\n";
    sn.allocate(img.get_width(), img.get_height(), img.get_offset(), src);

    std::cout << "finished allocating.\n";
    std::cout << "verifying...\n";
    std::cout << "offset: " << img.get_offset() << std::endl;
    sn.verify(img.get_width(), img.get_height(), img.get_offset(), src);
    std::cout << "verified.\n";

    return 0;
}