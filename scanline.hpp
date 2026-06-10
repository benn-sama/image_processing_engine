#ifndef SCANLINE_HPP
#define SCANLINE_HPP

#include <cstddef>
#include <random>
#include <sys/types.h>
#include <vector>
class Scanline {
    private:
        u_int8_t type; // 1 = grayscale, 2 = RGB, 3 = RGBA
        std::vector<std::vector<u_int8_t>> new_img;
        std::vector<std::vector<u_int8_t>> old_img;

        /*
        sub(x) = raw(x) - raw(x - bpp)
        
        Ref:
        x      = 0 -> # of bytes of that scanline - 1
        raw()  = raw data byte at that byte pos
        bpp    = # of bytes per pixel complete, rounding up to one (channel x bit_depth) / 8
        */
        void sub();
    public:
        void allocate(size_t x, size_t y); // allocates vector
        void filter();
};

#endif