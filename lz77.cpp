#include "lz77.hpp"
#include <cstdio>
#include <cstring>
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


void LZ77::compress() {
    // int const LOOK_AHEAD_SIZE = 4;
    // int const CHAR_SIZE       = 4;
    std::unordered_map<u_int16_t, int> bufferHash;
    u_int16_t windowPackage    = pack(buffer[0], buffer[1], buffer[2], buffer[3]);
    u_int16_t lookaheadPackage = pack(buffer[4], buffer[5], buffer[6], buffer[7]);

    for (int i = 8; i < int(buffer.size()) - 8; ++i ) {
        // window
        // std::cout << "window:     " << buffer[i] << buffer[i + 1] << buffer[i + 2] << buffer[i + 3] << std::endl;
        // if (buffer[i]     == buffer[i + 4] && 
        //     buffer[i + 1] == buffer[i + 5] && 
        //     buffer[i + 2] == buffer[i + 6] && 
        //     buffer[i + 3] == buffer[i + 7]) {
            
        // }
        auto it = bufferHash.find(windowPackage);
        if (it == bufferHash.end()) {
            bufferHash[windowPackage] = 0;
        } else {
            // code to make a tree here
            // something like:
            // cData.insert(<current - foundPos, length, nextChar>) 
        }
        

        // // right lookahead
        // std::cout << "look-ahead: " << buffer[i + 4] << buffer[i + 5] << buffer[i + 6] << buffer[i + 7] << std::endl;
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