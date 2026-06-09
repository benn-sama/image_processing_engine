#include "lz77.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <exception>
#include <functional>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

/*
 * This will pack the bytes, into one single 32x
 */
u_int32_t pack(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
    return (u_int32_t)a << 24 
         | (u_int32_t)b << 16
         | (u_int32_t)c << 8
         | (u_int32_t)d; 
}

// shifts bits to the left and assigns newByte into 
void enqueue(u_int32_t& package, unsigned char newByte) {
    package = (package << 8) | newByte; // shifts by 8, then 'appends' new byte to 0-8
}

// this is here for reference
// void dequeue(u_int32_t& package) {
//     // unsigned char byte = (unsigned char)(package >> 24); 
//     package <<= 8; // this joint shifts to out of bounds and drops the furthest left
// }

LZ77::LZ77() {}

void LZ77::parse() {
    std::string line;

    if (!file.is_open()) {
        std::cout << "File not open." << std::endl;
    }

    // assign var to the content of the buffer/file 
    std::stringstream fileBuffer;
    fileBuffer << file.rdbuf();
    std::string content = fileBuffer.str();

    // split content into unsigned chars
    buffer = std::vector<unsigned char>(content.begin(), content.end());
}
/*
 * Dynamic LZ77 algorithm
 */
void LZ77::compress() {
    size_t       current_index           = 0;
    size_t       search_window_tail      = MAX_WINDOW_SIZE;
    size_t       search_window_len = 8;
    std::unordered_map<std::string, int, string_hash, std::equal_to<>> map;

    
    while (current_index < size_t(buffer.size())) {
        for (; current_index < search_window_tail; ++current_index) {
            std::string_view viewPackage(reinterpret_cast<const char*>(buffer.data()) + current_index, std::min(search_window_len, buffer.size()));
            
            auto it = map.find(viewPackage);
            if (it == map.end()) {
                map.emplace(std::string(viewPackage), current_index);
                cData.emplace_back(current_index, 0, 0, buffer[current_index + 1]);
            } else {
                cData.emplace_back(Data{(u_int8_t)current_index, u_int8_t(current_index - it->second), 8, buffer[current_index + 1]});
                map.emplace(viewPackage, current_index);
                current_index += 7;
            }
        }

        // quick check if current search window length can support that last numbers
        std::cout << search_window_tail + MAX_WINDOW_SIZE  << ":" << size_t(buffer.size()) << std::endl;  
        if (search_window_tail + MAX_WINDOW_SIZE <= size_t(buffer.size())) {
            search_window_tail += MAX_WINDOW_SIZE;
        } else {
            search_window_tail += (size_t(buffer.size()) - search_window_tail);
            search_window_len  = search_window_tail;
        }

        map.clear();
    }
}

void LZ77::print() {

    for (auto& data : cData) {
        data.print();
    }
}

void LZ77::printE() {
    for (auto& chara : buffer) {
        std::cout << chara << ' ';
    }
}