#include "lz77.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <exception>
#include <functional>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

// packs 4 bytes into a 32 bit int
// u_int32_t package_bytes(u_int8_t one, u_int8_t two, u_int8_t three, u_int8_t four) {
//     return (static_cast<u_int32_t>(four)  << 24) |
//            (static_cast<u_int32_t>(three) << 16) |
//            (static_cast<u_int32_t>(two)   << 8 ) |
//            (static_cast<u_int32_t>(one)        );
// }

// dynamic bit packer
// feeling like genius
// u_int32_t package_bytes(std::vector<unsigned char>& data_stream, int const START_POS, int const OFFSET) {
//     u_int32_t package;

//     for (int i = START_POS; i < START_POS + OFFSET; ++i) {
//         package <<= 8;
//         package |= static_cast<u_int32_t>(data_stream[i]);
//     }

//     return package;
// }


LZ77::LZ77() {}

/*
This parses the data

*/
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
 * NOTE:
 * Assume 4 byte hash chain
 */
void LZ77::compress(std::vector<unsigned char>& data_stream, int const ARR_SIZE) {
    size_t head = 0;
    size_t tail = 4;
    size_t window_end = ARR_SIZE < MAX_WINDOW_SIZE ? ARR_SIZE : MAX_WINDOW_SIZE;
    std::unordered_map<u_int32_t, Token> map;
    
    // actual lz77 compression methods
    // NOTE: need to take account for the offset
    while (head < ARR_SIZE) {
        for (; head < window_end; ++head) {
            int length = window_end - head;
            if (length < 4) {
                head = length;
            }

            u_int32_t package = package_bytes(data_stream, head, length);
            auto it = map.find(package);
            Token new_token = Token{.is_match = true, .literal = package};

            // checks if Token already exists in the hashmap
            if (it == map.end()) { // not found
                new_token.is_match = false;
                map.emplace(new_token.literal, new_token);
                token_buffer.emplace_back(new_token);
            } else { // found
                // initialize new values
                new_token.match.position = head - it->second.match.position;
                new_token.match.length   = length; // default value always

                map.emplace(new_token.literal, new_token);
                token_buffer.emplace_back(new_token);//(u_int8_t)current_index, u_int8_t(current_index - it->second), 8, buffer[current_index + 1]});
                if (length > 4) {
                    head += 3;
                }
            }
        }

        // quick check if current search window length can support that last numbers
        if (window_end + MAX_WINDOW_SIZE <= ARR_SIZE) {
            window_end += MAX_WINDOW_SIZE;
        } else {
            window_end += ARR_SIZE - window_end;
            tail = window_end;
        }

        // restarts the hash map
        map.clear();
    }
}

void LZ77::print() {

    // for (auto& data : cData) {
    //     data.print();
    // }
}

void LZ77::printE() {
    for (auto& chara : buffer) {
        std::cout << chara << ' ';
    }
}