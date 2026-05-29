#include "lz77.hpp"
#include <cstdio>
#include <cstring>
#include <sys/types.h>
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
    size_t currIndex = 0;

    while (currIndex < buffer.size()) {
        int bestLen = 0;
        int bestPos = 0;

        // search window: [start, currIndex)
        size_t winStart = (currIndex >= HALF_MAX_WINDOW_SIZE) ? currIndex - HALF_MAX_WINDOW_SIZE : 0;

        for (size_t i = winStart; i < currIndex; ++i) {
            int len = 0;
            // extend match
            while (currIndex + len < buffer.size() && buffer[i + len] == buffer[currIndex + len]) {
                ++len;
            }
            
            if (len > bestLen) {
                bestLen = len;
                bestPos = currIndex - i;  // how far back
            }
        }

        u_int8_t nextChar = (currIndex + bestLen < buffer.size())
                            ? buffer[currIndex + bestLen]
                            : 0;

        cData.emplace_back(Data{
            u_int8_t(bestPos),
            u_int8_t(bestLen),
            nextChar
        });

        currIndex += bestLen + 1;
    }

    for (auto& element : cData) {
        element.print();
    }

    // int currIndex = 0;
    // Data data = {0, 0, 'a'};
    // bool isExit = false;

    // while (currIndex < int(buffer.size())) {
    //     for (int i = currIndex; i < currIndex + HALF_MAX_WINDOW_SIZE; ++i) {
    //         Data data = {0, 0, 'a'};
    //         int bestLen = 0;
    //         int bestPos = 0;

    //         for (int j = currIndex + HALF_MAX_WINDOW_SIZE; j < currIndex + MAX_WINDOW_SIZE; ++j) {
    //             while (std::memcmp(buffer.data() + i, buffer.data() + j, length * sizeof(int)) == 0) {
    //                 bestPos = j - i;
    //                 ++bestLen;
    //                 ++length;
    //                 std::cout << buffer[i] << ":" << buffer[j] << " - " << i << ":" << j << std::endl;
    //                 std::cout << "<" << bestPos << ", " << bestLen << ">" << buffer[j] << std::endl; 
    //                 isExit = true;
    //             }
    //             data.pos  = u_int8_t(bestPos);
    //             data.len  = u_int8_t(bestLen);
    //             data.code = u_int8_t(buffer[j]); 
    //             cData.emplace_back(data);

    //             if (isExit) {
    //                 isExit = false;
    //                 break;
    //             }
    //         }

    //         bestPos = 0;
    //         bestLen = 0;
    //         length = 1;
    //     }
    //     currIndex += HALF_MAX_WINDOW_SIZE;
    // }

    // for (auto& temp : cData) {
    //     temp.print();
    // }
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