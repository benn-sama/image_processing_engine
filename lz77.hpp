#ifndef LZ77_HPP
#define LZ77_HPP
// this will be a test file to mess around with the lz77 algorithm
// "sliding window"
// if no match is found, <offset=0, length=0>word
// offset = the ptr of the beginning of the word

// static var = lives for the whole lifetime of the program

#include <array>
#include <cstddef>
#include <fstream>
#include <sys/types.h>
#include <vector>
#include <iostream>
#include <sstream>

struct Data {
    u_int8_t pos;
    u_int8_t len;
    u_int8_t code;

    void print() {
        std::cout << "<" << (int)pos << ", " << (int)len << ">" << (char)code << std::endl;
    }
};

class LZ77 {
    static const size_t                    MAX_WINDOW_SIZE = 64;     // window size of the LZ77 window
    std::array<u_int8_t, MAX_WINDOW_SIZE>  window;  // the actual LZ77 window

    u_int16_t const                        HALF_MAX_WINDOW_SIZE  = 32; // 0-127 and 128-256
    u_int16_t                              length = 1;
    
    std::ifstream file{"story-test.txt"};
    std::vector<unsigned char> buffer;         // the thing being scanned
    std::vector<Data> cData;

    public:
        LZ77();
        void parse(); // file content is split into unsigned chars
        void compress();
        void print();
        void printE();
};

#endif