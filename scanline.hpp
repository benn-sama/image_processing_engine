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
        std::vector<std::vector<unsigned char>> new_img;
        std::vector<std::vector<unsigned char>> old_img;

        /*
        sub(x) = raw(x) - raw(x - bpp)
        
        Ref:
        x      = 0 -> # of bytes of that scanline - 1
        raw()  = raw data byte at that byte pos
        bpp    = # of bytes per pixel complete, rounding up to one (channel x bit_depth) / 8
        */
        void sub();
    public:
        void allocate(size_t const x, size_t const y, long const offset, std::fstream* buffer_stream); // allocates vector
        void filter();
        void verify(size_t const width, size_t const height, long const offset, std::fstream* buffer_stream);
};

#endif