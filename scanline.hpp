#ifndef SCANLINE_HPP
#define SCANLINE_HPP

#include <cstddef>
#include <fstream>
#include <random>
#include <sys/types.h>
#include <vector>
#include <iostream>
#include <memory>

class Scanline {
    private:
        u_int8_t type; // 1 = grayscale, 2 = RGB, 3 = RGBA
        std::vector<std::vector<unsigned char>> _new_img;
        std::vector<std::vector<unsigned char>> _old_img;

        size_t _width  = 0;
        size_t _height = 0;
        size_t _channel = 3; // this needs to be dynamic, 3 = RGB, how many channels
        size_t _bit_depth = 8; // how many bits in one channel


        /*
        sub(x) = raw(x) - raw(x - bpp)
        
        Ref:
        x      = 0 -> # of bytes of that scanline - 1
        raw()  = raw data byte at that byte pos
        bpp    = # of bytes per pixel complete, rounding up to one (channel x bit_depth) / 8
        */
        public:
        void allocate(size_t const width, size_t const height, long const offset, std::fstream* buffer_stream); // allocates vector
        void filter();
        void verify(size_t const width, size_t const height, long const offset, std::fstream* buffer_stream);
        void sub();
        int subf(int const current_byte, int const bpp); // the math func for sub
        void print_new_img() {
            for (int i = 0; i < _width; ++i) {
                for (int j = 0; j < _height; ++j) {
                    std::cout << "[" << i << "]" << "[" << j << "]" << " = " << (int)_new_img[i][j] << std::endl; 
                }
            }
        };
};

#endif