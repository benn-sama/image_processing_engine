#include "scanline.hpp"
#include <array>
#include <cstddef>
#include <fstream>
#include <sys/types.h>

// this shouldn't assume that it is offset, must fix later
void Scanline::allocate(size_t const width, size_t const height, long const offset, std::fstream* buffer_stream) {
    std::unique_ptr buffer = std::make_unique<char[]>(1);
    // w = 2268, h = 4032
    std::cout << "width: " << width << std::endl << "height: " << height << std::endl;

    for (size_t i = 0; i < width; ++i) {
        old_img.push_back({}); // create a row before accessing
        for (size_t j = 0; j < height; ++j) {
            buffer_stream->read(buffer.get(), 1);
            old_img[i].push_back(buffer[0]);
            std::cout << "pushed: " << "[" << i << "]" << "[" << j << "]" << " = " << (int)old_img[i][j] << std::endl;
        }
    }
    
    // points ptr to the offset 
    buffer_stream->seekg(offset, std::ios::beg);
}

void Scanline::verify(size_t const width, size_t const height, long const offset, std::fstream* buffer_stream) {
    std::unique_ptr buffer = std::make_unique<char[]>(1);

    for (size_t i = 0; i < width; ++i) {
        for (size_t j = 0; j < height; ++j) {
            buffer_stream->read(buffer.get(), 1);
            if (old_img[i][j] != (u_int8_t)buffer[0]) {
                std::cout << "[" << i << "]" << "[" << j << "] " << " = " << (int)old_img[i][j] << " != " << (u_int8_t)buffer[0] << std::endl;
                exit(1);
            }
        }
    }

    buffer_stream->seekg(offset, std::ios::beg);
}