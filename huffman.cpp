#include "huffman.hpp"
#include "lz77.hpp"

// counts the occurrence of each byte in &buffer_stream
void Huffman::count(std::vector<Token>& buffer_stream, int const STREAM_SIZE) {
    for (int i = 0; i < STREAM_SIZE; ++i) {
        auto it = byte_counter.find(buffer_stream[i]);

        if (it != byte_counter.end()) {
            ++it->second;
        } else {
            byte_counter.emplace(buffer_stream[i], 1);
        }
    }
}

// extracts tokens with count occurrence
void Huffman::extract_count(std::vector<TokenC>& tokens) {
    tokens.reserve(byte_counter.size());
    for (const auto& [key, value] : byte_counter) {
        tokens.push_back(TokenC{key, value});
    }
}