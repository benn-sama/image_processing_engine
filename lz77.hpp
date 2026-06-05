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

struct Data {
    u_int8_t index;
    u_int8_t pos;
    u_int8_t len;
    u_int8_t code;

    void print() {
        std::cout << (int)index << "<" << (int)pos << ", " << (int)len << ">" << (unsigned char)code << std::endl;
    }
};

// 258 matchingWindowSize
struct alignas(4096) Package {
    std::array<u_int8_t, 32768> window; // 32 * 1024 = 32KB
    size_t currentIndex = 0;
    
    std::unordered_map<std::string, u_int8_t> map;

};

// allows string view to allocate a string
struct string_hash {
    using is_transparent = void; // opt-in marker
    size_t operator()(std::string_view sv)  const { return std::hash<std::string_view>{}(sv); }
    size_t operator()(const std::string& s) const { return std::hash<std::string_view>{}(s); }
    size_t operator()(const char* s)        const { return std::hash<std::string_view>{}(s);}
};

class LZ77 {
    static constexpr size_t MAX_WINDOW_SIZE = 64;     // window size of the LZ77 window

    u_int16_t length = 1;
    
    std::ifstream file{"story-test.txt"};
    std::vector<unsigned char> buffer;         // the thing being scanned
    std::vector<Data> cData;
    std::array<u_int8_t, 64> window; // 32768

    public:
        LZ77();
        void parse(); // file content is split into unsigned chars
        void compress();
        void compress2();
        void print();
        void printE();
};

#endif