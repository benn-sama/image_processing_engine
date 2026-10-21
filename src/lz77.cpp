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
void LZ77::compress(std::vector<unsigned char>& data_stream, std::vector<Token> token_buffer, int const ARR_SIZE) {
    size_t head = 0;
    std::unordered_map<u_int32_t, size_t> map;
    // proceeds to get one byte at a time
    auto emit_literal = [&]() {
       token_buffer.emplace_back(Token{.is_match = false, .literal = data_stream[head]});
       ++head; 
    };

    // actual lz77 compression method
    while (head < (size_t)ARR_SIZE) {
        // gets the remaining bytes at the end of the sliding window
        if ((size_t)ARR_SIZE - head < 4) { 
            emit_literal(); 
            continue;
        }

        u_int32_t package = package_bytes(data_stream, head, 4);
        auto it = map.find(package);

        // walks the found chain to find the next largest repetitive sequence
        size_t match_len = 0;
        if (it != map.end() && head - it->second <= (size_t)MAX_WINDOW_SIZE) {
            size_t max_len = std::min((size_t)ARR_SIZE - head, (size_t)32768); // checks for overflow
            while (match_len < max_len && data_stream[it->second + match_len] == data_stream[head + match_len] && match_len < 258) {
                ++match_len;
            }
        }

        size_t prev_pos = (it != map.end()) ? it->second : 0;
        map[package] = head; // always update to the most recent pos

        if (match_len < 4) { emit_literal(); continue; }

        // build the repetitive token
        Token t{};
        t.is_match = true;
        t.match.position = (u_int16_t)(head - prev_pos);
        t.match.length   = (u_int16_t)match_len;
        token_buffer.emplace_back(t);
        head += match_len;
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