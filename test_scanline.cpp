#include "scanline.hpp"
#include "image_processor.hpp"
#include <memory>
#include <unistd.h>

void test_subf() {
    Scanline sn;

// bpp == 0 short-circuit
    if (sn.subf(10, 0) != 0) {
        std::cout << "10:0 is not 0, got " << sn.subf(10, 0) << std::endl;
    }

    // normal positive cases
    if (sn.subf(10, 3) != 7) {
        std::cout << "10:3 is not 7, got " << sn.subf(10, 3) << std::endl;
    }
    if (sn.subf(3, 3) != 0) {
        std::cout << "3:3 is not 0, got " << sn.subf(3, 3) << std::endl;
    }

    // underflow cases (current_byte < bpp) -- expected values assume SIGNED int
    if (sn.subf(0, 1) != -1) {   // unsigned would be 255
        std::cout << "0:1 is not -1, got " << sn.subf(0, 1) << std::endl;
    }
    if (sn.subf(1, 4) != -3) {   // unsigned would be 253
        std::cout << "1:4 is not -3, got " << sn.subf(1, 4) << std::endl;
    }
    if (sn.subf(2, 4) != -2) {   // unsigned would be 254
        std::cout << "2:4 is not -2, got " << sn.subf(2, 4) << std::endl;
    }

    // wrap boundaries
    if (sn.subf(259, 3) != 0) {
        std::cout << "259:3 is not 0, got " << sn.subf(259, 3) << std::endl;
    }
    if (sn.subf(258, 3) != 255) {
        std::cout << "258:3 is not 255, got " << sn.subf(258, 3) << std::endl;
    }
    if (sn.subf(260, 3) != 1) {
        std::cout << "260:3 is not 1, got " << sn.subf(260, 3) << std::endl;
    }

    // large input
    if (sn.subf(1000, 8) != 224) {
        std::cout << "1000:8 is not 248, got " << sn.subf(1000, 8) << std::endl;
    }
}

void test_sub() {
    Scanline sn;
    Image    img;

    std::string str = "./photos/hills.ppm"; 
    img.source(str);
    std::fstream* src = img.get_ptr();

    sn.allocate(img.get_width(), img.get_height(), img.get_offset(), src);
    sn.sub();
    sn.print_new_img();
}

void test_scanline() {
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
}


int main() {
    // test_sub();
    test_sub();
    return 0;
}