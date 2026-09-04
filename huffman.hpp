#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <cstdint>
#include <sys/types.h>
#include <unordered_map>
#include "lz77.hpp"
#include <algorithm>

// make clean; make tests; ./tests/test_huffman

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

struct Code {
    u_int8_t  code_len;
    u_int16_t code;
};

typedef struct {
    u_int16_t code;
    u_int16_t len;
    u_int16_t min;
    u_int16_t max;
} Length_To_Code;

static constexpr u_int8_t LEN_TABLE_SIZE = 29;
static constexpr Length_To_Code len_table[29] {
    {257,0,3,3},     {258,0,4,4},     {259,0,5,5},     {260,0,6,6},
    {261,0,7,7},     {262,0,8,8},     {263,0,9,9},     {264,0,10,10},
    {265,1,11,12},   {266,1,13,14},   {267,1,15,16},   {268,1,17,18},
    {269,2,19,22},   {270,2,23,26},   {271,2,27,30},   {272,2,31,34},
    {273,3,35,42},   {274,3,43,50},   {275,3,51,58},   {276,3,59,66},
    {277,4,67,82},   {278,4,83,98},   {279,4,99,114},  {280,4,115,130},
    {281,5,131,162}, {282,5,163,194}, {283,5,195,226}, {284,5,227,257}, // <-- capped at 257, not 258!
    {285,0,258,258}
};

static constexpr u_int8_t DIST_TABLE_SIZE = 30;
static constexpr Length_To_Code dist_table[30] {
    {0,  0, 1,     1},     {1,  0, 2,     2},     {2,  0, 3,     3},     {3,  0, 4,     4},
    {4,  1, 5,     6},     {5,  1, 7,     8},     {6,  2, 9,     12},    {7,  2, 13,    16},
    {8,  3, 17,    24},    {9,  3, 25,    32},    {10, 4, 33,    48},    {11, 4, 49,    64},
    {12, 5, 65,    96},    {13, 5, 97,    128},   {14, 6, 129,   192},   {15, 6, 193,   256},
    {16, 7, 257,   384},   {17, 7, 385,   512},   {18, 8, 513,   768},   {19, 8, 769,   1024},
    {20, 9, 1025,  1536},  {21, 9, 1537,  2048},  {22, 10, 2049, 3072},  {23, 10, 3073, 4096},
    {24, 11, 4097, 6144},  {25, 11, 6145, 8192},  {26, 12, 8193, 12288}, {27, 12, 12289,16384},
    {28, 13, 16385,24576}, {29, 13, 24577,32768}
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

/*
This struct is design to hold to total number of occurence of at literal/length
AND
the element of it.

Why?
So we can keep track of the literal AND the occurence becuase the element_id represents a literal too
*/
struct Tally {
    int total_occurence = 0;
    int element_id = 0;

    // underneath increments everytime a new instance is created
    inline static int counter = 0;
    Tally() : element_id(++counter) {}
};

class Huffman {
    private:
    public:
        // counts the occurence of literal and length
        // NOTE: all ranges are inclusive
        // literal             = 0-255
        // end of block marker = 256
        // length              = 257-285
        std::array<Tally, 286> lit_len_counter_actual = {};
        std::array<Tally, 286> lit_len_counter_dupe = {};
        std::vector<u_int16_t>     huffman_tree; // this is the huffman tree, might need to change ds later
        // counts the occurence of distance (pairs only)
        // NOTE: all ranges are inclusive
        // distance                         = 0-29
        // included but probably never seen = 30-31
        std::array<u_int16_t, 32>  dist_counter;

        Huffman() {};
        u_int16_t count_occurrences(std::vector<Token>& buffer_stream, int const STREAM_SIZE);
        u_int16_t get_len_code(u_int16_t const length);
        u_int16_t get_dist_code(u_int16_t const distance);
        void reset_len_len_counter();
        void sort();
        void generate_huffman_tree();
};

#endif