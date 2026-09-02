#include "huffman.hpp"
#include "lz77.hpp"
#include <functional>
#include <sys/types.h>

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
u_int16_t Huffman::count_occurrences(std::vector<Token>& buffer_stream, int const STREAM_SIZE) {
    for (int i = 0; i < STREAM_SIZE; ++i) {
        if (!buffer_stream[i].is_match) {
            ++lit_len_counter[buffer_stream[i].literal].total_occurence;
        } else {
            u_int16_t code = get_code(buffer_stream[i].match.length);
            if (code == 259) { return code; }
            ++lit_len_counter[code].total_occurence;
        }
    }

    lit_len_counter[256].total_occurence = 1;

    return 0;
}

/*
What this does:
  This is ONLY used WHEN a LZ77 pair is the current element.
  This will find where that length appears in the length map using a min max range and return that CODE.
  From length 3-258 (inclusive) can be mapped.
  
  Returns 259 IF it is out of range. (P.S. This shouldn't return 259 ever because LZ77 needs to make sure length is in between 3-258 inclusive)
*/
u_int16_t Huffman::get_code(u_int16_t const length) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (length >= len_table[i].min && length <= len_table[i].max) {
            return len_table[i].code;
        }
    }

    return 259;
}

/*
What this does:
  This should be ran AFTER Huffman coding is COMPLETELY Finished
  This zeros ALL THE ARRAYS to prepare it for the next Block stream
*/
void Huffman::zero_all() {
    for (int i = 0; i < int(lit_len_counter.size()); ++i) {
        lit_len_counter[i].total_occurence = 0;
    }
}

void Huffman::sort() {
    // sort ascending order to start assigning actual code
    std::sort(lit_len_counter.begin(), lit_len_counter.end(),
             [](const Tally &a, const Tally &b) {
                return a.total_occurence > b.total_occurence;
             });
}