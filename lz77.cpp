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
u_int32_t package_bytes(u_int8_t one, u_int8_t two, u_int8_t three, u_int8_t four) {
    return (static_cast<u_int32_t>(four)  << 24) |
           (static_cast<u_int32_t>(three) << 16) |
           (static_cast<u_int32_t>(two)   << 8 ) |
           (static_cast<u_int32_t>(one)        );
}

// dynamic bit packer
// feeling like genius
u_int32_t package_bytes(std::vector<unsigned char>& data_stream, int const START_POS, int const OFFSET) {
    u_int32_t package;

    for (int i = START_POS; i < START_POS + OFFSET; ++i) {
        package <<= 8;
        package |= static_cast<u_int32_t>(data_stream[i]);
    }

    return package;
}

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
    size_t i                  = 0;
    size_t search_window_tail = ARR_SIZE < MAX_WINDOW_SIZE ? ARR_SIZE : MAX_WINDOW_SIZE;
    size_t search_window_len  = 4;
    std::unordered_map<u_int32_t, Token> map;
    
    // actual lz77 compression methods
    // NOTE: need to take account for the offset
    while (i < ARR_SIZE) {
        for (; i < search_window_tail - 4; ++i) {
            int length = search_window_tail - i;
            if (length < 4) {
                i -= length;
            }

            u_int32_t package = package_bytes(data_stream[i], data_stream[i + 1], data_stream[i + 2], data_stream[i + 3]);
            auto it = map.find(data_stream[i]);
            Token new_token = Token{.is_match = true, .literal = package};

            // checks if Token already exists in the hashmap
            if (it == map.end()) { // not found
                new_token.is_match = false;
                map.emplace(new_token.literal, new_token);
                token_buffer.emplace_back(new_token);
            } else { // found
                // initialize new values
                new_token.match.position = i - it->second.match.position;
                new_token.match.length   = 4; // default value always

                map.emplace(new_token.literal, new_token);
                token_buffer.emplace_back(new_token);//(u_int8_t)current_index, u_int8_t(current_index - it->second), 8, buffer[current_index + 1]});
                i += 3;
            }
        }

        // quick check if current search window length can support that last numbers
        if (search_window_tail + MAX_WINDOW_SIZE <= ARR_SIZE) {
            search_window_tail += MAX_WINDOW_SIZE;
        } else {
            search_window_tail += ARR_SIZE - search_window_tail;
            search_window_len  = search_window_tail;
        }

        // restarts the hash map
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