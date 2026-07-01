#ifndef FILTER_HPP
#define FILTER_HPP

#include <array>
#include <cstddef>
#include <fstream>
#include <random>
#include <sys/types.h>
#include <vector>
#include <iostream>
#include <memory>

// this should be renamed to filter

class Filter {
    private:
        size_t _bpp       = 4;

        /*
        sub(x) = raw(x) - raw(x - bpp)
        
        Ref:
        x      = 0 -> # of bytes of that scanline - 1
        raw()  = raw data byte at that byte pos
        bpp    = # of bytes per pixel complete, rounding up to one (channel x bit_depth) / 8
        */
        public:
        Filter() {};
        Filter(size_t const channel, size_t const bit_depth);
        
        // algorithms for compressions specifically
        void sub(std::vector<unsigned char>& bottom, std::vector<unsigned char>& subv, int const ARR_SIZE);
        void up(std::vector<unsigned char>& bottom, std::vector<unsigned char>& top, std::vector<unsigned char>& upv, int const ARR_SIZE);
        void avg(std::vector<unsigned char>& bottom, std::vector<unsigned char>& top, std::vector<unsigned char>& avgv, int const ARR_SIZE );
        void paeth(std::vector<unsigned char>& bottom, std::vector<unsigned char>& top, std::vector<unsigned char>& paethv, int const ARR_SIZE);
        void filter_scanline(std::vector<unsigned char>& top, std::vector<unsigned char>& bottom, std::vector<unsigned char>& alter, int const ARR_SIZE);
        
        // subf prolly needs to to priv or just needs to be seperated into it's own func rather than it being owned by a class
        int subf(int const current_byte, int const go_back_n); // the math func for sub
        int upf(int const current_byte, int const prior);
        int avgf(int const current_byte, int const prior, int const go_back_n);
        int paethf(int const up, int const left, int const top_left);

        // this needs to be deleted  in the future (this is for testing purposes only )
        void print_new_img() {
            for (int i = 0; i < _width; ++i) {
                for (int j = 0; j < _height; ++j) {
                    std::cout << "[" << i << "]" << "[" << j << "]" << " = " << (int)_new_img[i][j] << std::endl; 
                }
            }
        };

        void print_old_img() {
            for (int i = 0; i < _width; ++i) {
                for (int j = 0; j < _height; ++j) {
                    std::cout << "[" << i << "]" << "[" << j << "]" << " = " << (int)_old_img[i][j] << std::endl;
                }
            }
        }
};

#endif