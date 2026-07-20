#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <sys/types.h>
#include <unordered_map>
#include "lz77.hpp"

typedef struct {
    u_int16_t base;
    u_int8_t  extra_bits;
} CodeEntry;

static constexpr CodeEntry length_table[29] = {
    {3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},{10,0},
    {11,1},{13,1},{15,1},{17,1},
    {19,2},{23,2},{27,2},{31,2},
    {35,3},{43,3},{51,3},{59,3},
    {67,4},{83,4},{99,4},{115,4},
    {131,5},{163,5},{195,5},{227,5},
    {258,0}
};

// index with (symbol - 0), i.e. just the distance symbol directly
static constexpr CodeEntry dist_table[30] = {
    {1,0},{2,0},{3,0},{4,0},
    {5,1},{7,1},
    {9,2},{13,2},
    {17,3},{25,3},
    {33,4},{49,4},
    {65,5},{97,5},
    {129,6},{193,6},
    {257,7},{385,7},
    {513,8},{769,8},
    {1025,9},{1537,9},
    {2049,10},{3073,10},
    {4097,11},{6145,11},
    {8193,12},{12289,12},
    {16385,13},{24577,13}
};

struct Code {
    u_int8_t  code_len;
    u_int16_t code;
};

// token_hash.hpp
//
// Token has no operator== or std::hash<Token>, which is why
// std::unordered_map<Token, int> fails to instantiate. Add these
// wherever Token is defined (or #include this file before huffman.hpp
// and before Token is ever used as a map/set key).
//
// Semantics chosen:
//   - Two tokens are only equal if is_match matches.
//   - If is_match == false, compare by `literal`.
//   - If is_match == true, compare by (position, length).
// This avoids reading the inactive union member (reading `literal`
// while `match` is active, or vice versa, is technically UB even
// though it "works" on most compilers).

#pragma once
#include <functional>

inline bool operator==(const Token& a, const Token& b) {
    if (a.is_match != b.is_match) {
        return false;
    }
    if (a.is_match) {
        return a.match.position == b.match.position &&
               a.match.length   == b.match.length;
    }
    return a.literal == b.literal;
}

namespace std {
    template<>
    struct hash<Token> {
        size_t operator()(const Token& t) const noexcept {
            size_t h;
            if (t.is_match) {
                size_t h1 = std::hash<u_int16_t>{}(t.match.position);
                size_t h2 = std::hash<u_int16_t>{}(t.match.length);
                h = h1 ^ (h2 * 0x9e3779b97f4a7c15ULL + 0x1);
            } else {
                h = std::hash<u_int32_t>{}(t.literal);
            }
            // fold in is_match so a match and a literal can never collide
            // into "equal" territory even if their bit patterns match
            return h ^ (static_cast<size_t>(t.is_match) << 1);
        }
    };
}

struct TokenC {
    Token token;
    int   count = 0;
};

class Huffman {
    private:
    public:
        std::unordered_map<Token, int> byte_counter;
        std::array<
        Huffman() {};
        void count(std::vector<Token>& buffer_stream, int const STREAM_SIZE);
        void extract_count(std::vector<TokenC>& tokens);
};

#endif