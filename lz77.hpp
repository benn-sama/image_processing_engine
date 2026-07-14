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
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>

struct Token {
    bool is_match;

    union {
        u_int32_t literal;

        struct {
            u_int8_t position = 0;
            u_int8_t length   = 0;
        } match;
    };

    void print() {
        if (is_match) {
            std::cout << "<" << (int)match.position << ", " << (int)match.length << ">" << (unsigned char)literal << std::endl;
        } else {
            std::cout << (unsigned char)literal << '\n';
        }
    }
};

u_int32_t package_bytes(std::vector<unsigned char>& data_stream, int const START_POS, int const OFFSET) {
    u_int32_t package = 0;

    for (int i = START_POS; i < START_POS + OFFSET; ++i) {
        package <<= 8;
        package |= static_cast<u_int32_t>(data_stream[i]);
    }

    return package;
}

class LZ77 {
    private:
        static constexpr int MAX_WINDOW_SIZE = 8000;     // window size of the LZ77 window = 8KB

        u_int16_t length = 1;
    
        std::ifstream file{"story.txt"};
        std::vector<unsigned char> buffer;         // the thing being scanned
        std::vector<Token> token_buffer;
        std::array<u_int8_t, 64> window; // 32768

    public:
        LZ77();
        void parse(); // file content is split into unsigned chars
        void compress(std::vector<unsigned char>& data_stream, int const ARR_SIZE);
        void compress2();
        void print();
        void printE();
};

#endif