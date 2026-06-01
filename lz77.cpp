#include "lz77.hpp"
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

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

// void LZ77::compress() {
//     int currIndex = 0;

//     // this is a better refactor, but holy ts def can be much better
//     int i = currIndex;
//     bool temp = false;
//     while (currIndex < int(buffer.size())) {

//         // RIGHT SIDE
//         for (int j = currIndex + HALF_MAX_WINDOW_SIZE; j < currIndex + MAX_WINDOW_SIZE; ++j) {
//             Data data = {0, 0, buffer[j]};
//             std::cout << "In for loop\n";

//             // assumes that if a match, then it keeps checking if there is a bigger match
//             while (std::memcmp(buffer.data() + i, buffer.data() + j, length * sizeof(int)) == 0) {
//                 std::cout << "Caught a substr: " << buffer[j] << std::endl;
//                 std::cout << "Indexes -> " << i << ':' << j << std::endl;
//                 std::cout << buffer[i] << ":" << buffer[j] << std::endl;

//                 data.pos  = j - i;
//                 std::cout << "j - i = " << j - i << std::endl;
//                 data.len  = length;
//                 data.code = buffer[j];
//                 std::cout << buffer[j] << ": <" << (int)data.pos << ", " << (int)data.len << ">" << (int)data.code << std::endl;
//                 std::cout << "Length: " << length << std::endl;
//                 ++length;
//                 temp = true;
//             }

//             // this is here to break the for loop
//             if (temp) {
//                 cData.emplace_back(data);
//                 temp = false;
//                 std::cout << "BREAKING LOOP AFTER\n";
//                 std::cout << ": <" << (int)data.pos << ", " << (int)data.len << ">" << (int)data.code << std::endl;
//                 ++i;
//                 std::cout << "Increased i: " << i << std::endl;
//                 length = 1; // resets length
//                 break;
//             }

//             cData.emplace_back(data);
//             std::cout << ": <" << (int)data.pos << ", " << (int)data.len << ">" << (int)data.code << std::endl;
            
//         }
//         // moves the window by 256
//         if (currIndex % MAX_WINDOW_SIZE == 0) {
//             currIndex += MAX_WINDOW_SIZE;
//             std::cout << "currIndex increased: " << currIndex << std::endl;
//             i = currIndex;
//         }
        
//     }
// }

void LZ77::compress() {
    // int const LOOK_AHEAD_SIZE = 4;
    // int const CHAR_SIZE       = 4;
    std::unordered_map<unsigned char, int> windowBuffer;

    for (int i = 0; i < int(buffer.size()) - 8; ++i ) {
        // window
        std::cout << "window:     " << buffer[i] << buffer[i + 1] << buffer[i + 2] << buffer[i + 3] << std::endl;
        
        // right lookahead
        std::cout << "look-ahead: " << buffer[i + 4] << buffer[i + 5] << buffer[i + 6] << buffer[i + 7] << std::endl;
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


    // u_int16_t right = currIndex + HALF_MAX_WINDOW_SIZE;
    // u_int16_t left  = currIndex;
    // while (currIndex < int(buffer.size())) {
    //     length = 1;

    //     while (std::memcmp(buffer.data() + left, buffer.data() + right, length)) {
    //         ++length;
    //         ++right;
    //         ++left;
    //     }
    //     ++right;
    // }