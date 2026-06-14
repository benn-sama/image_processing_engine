#include "scanline.hpp"
#include <array>
#include <cstddef>
#include <fstream>
#include <sys/types.h>

// this shouldn't assume that it is offset, must fix later
void Scanline::allocate(size_t const width, size_t const height, long const offset, std::fstream* buffer_stream) {
    std::unique_ptr buffer = std::make_unique<char[]>(1);
    _width  = width;
    _height = height;
    
    // w = 2268, h = 4032
    std::cout << "width: " << width << std::endl << "height: " << height << std::endl;

    // stores the the old img, just in case for later
    // allocates new img, so we don't have to allocate memory, since it will always be allocated already
    for (size_t i = 0; i < width; ++i) {
        _old_img.push_back({}); // create a row before accessing
        _new_img.push_back({});
        for (size_t j = 0; j < height; ++j) {
            buffer_stream->read(buffer.get(), 1);
            _old_img[i].push_back(buffer[0]);
            _new_img[i].push_back(buffer[0]);
            // std::cout << "pushed: " << "[" << i << "]" << "[" << j << "]" << " = " << (int)_old_img[i][j] << std::endl;
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
            if (_old_img[i][j] != (u_int8_t)buffer[0]) {
                // std::cout << "[" << i << "]" << "[" << j << "] " << " = " << (int)_old_img[i][j] << " != " << (u_int8_t)buffer[0] << std::endl;
                exit(1);
            }
        }
    }

    buffer_stream->seekg(offset, std::ios::beg);
}

int Scanline::subf(int const current_byte, int const bpp) {
    return bpp == 0 ? 0 : (current_byte - bpp) % 256;
}

/*
sub(x) = [curr_byte - bpp] mod 256
bpp    = curr_byte then go back n (if x - 3 < 0, then it is 0)
n      = (channel * bit_depth) / 8
*/
void Scanline::sub() {
    for (size_t i = 0; i < _width; ++i) {
        for (size_t j = 0; j < _height; ++j) {
            int bpp = (j < 3) ? 0 : _old_img[i][j - 3];
            _new_img[i][j] = subf((int)_old_img[i][j], (int)bpp);
        }
    }
}

int Scanline::upf(int const current_byte, int const prior) {
    return current_byte < prior ? current_byte - prior  + 256: current_byte - prior;
}

/*
up() = raw(x) - prior(x)
prior(x) = byte of previous line of index j, [i - 1][j]
*/
void Scanline::up() {
    for (int i = 0; i < _width; ++i) {
        for (int j = 0; j < _height; ++j) {
            int prior = i <= 0 ? 0 : (int)_old_img[i - 1][j];
            _new_img[i][j] = upf((int)_old_img[i][j], prior);
        }
    }
}

int Scanline::paethf(int const current, int const floor) {
    
}