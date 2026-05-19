#include "image_processor.hpp"
#include <fstream>
#include <memory>
#include <type_traits>

Image::Image() {}

void Image::source(std::string dir) {
    src = std::make_unique<std::fstream>(
        dir,
        std::ios::in | std::ios::binary
    );
}

void Image::dest(std::string dir) {
    dst = std::make_unique<std::fstream>(
        dir,
        std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc
    );
}