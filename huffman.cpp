#include "huffman.hpp"
#include "lz77.hpp"
#include <sys/types.h>

// // counts the occurrence of each byte in &buffer_stream
// void Huffman::count(std::vector<Token>& buffer_stream, int const STREAM_SIZE) {
//     for (int i = 0; i < STREAM_SIZE; ++i) {
//         auto it = byte_counter.find(buffer_stream[i]);

//         if (it != byte_counter.end()) {
//             ++it->second;
//         } else {
//             byte_counter.emplace(buffer_stream[i], 1);
//         }
//     }
// }

// // extracts tokens with count occurrence
// void Huffman::extract_count(std::vector<TokenC>& tokens) {
//     tokens.reserve(byte_counter.size());
//     for (const auto& [key, value] : byte_counter) {
//         tokens.push_back(TokenC{key, value});
//     }
// }

/*
What this methods does:
  This counts the total occurences of literals that appear in buffer_stream and then add EOB (End Of Block)
  Two ways it can count an occurrence of Token in buffer_stream:

    1. If that token is only a literal (Ex. any byte)
        1. a. Increment by one in elemenets between 0-255 corresponding with the value of the token literal
        1. b. Get next token and continue

    2. If that token is a pair (Ex. <5. 100>)
        2. a. Extract only len from <len, dis>
        2. b. Map len to a specific code (use get_code method)
        2. c. Increment by one in elements between 257-285 corresponding with mapped value of the length to code
        2. d. Get next token and continue

  When it counts all elements in buffer_stream, append an EOB in 256.
*/
void Huffman::count_occurrences(std::vector<Token>& buffer_stream, int const STREAM_SIZE) {
    for (int i = 0; i < STREAM_SIZE; ++i) {
        if (!buffer_stream[i].is_match) {
            ++lit_len_counter[buffer_stream[i].literal];
        } else {
            u_int16_t code = get_code(buffer_stream[i].literal);
            if (code == 259) { return; } // there was an error in get_code()
            ++lit_len_counter[code];
        }
    }

    ++lit_len_counter[256];
}

u_int16_t Huffman::get_code(u_int16_t const length) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (length >= len_table[i].min && length <= len_table[i].max) {
            return len_table[i].code;
        }
    }

    return 259;
}