// test_huffman_count.cpp
//
// Unit tests for Huffman::count(std::vector<Token>&, int).
//
// ASSUMPTIONS (adjust if your huffman.hpp differs):
//   1. `byte_counter` is accessible from test code (public, or you add
//      a small accessor like `auto& counts() { return byte_counter; }`
//      to the Huffman class for testing purposes).
//   2. Token has a working hash/equality so it can be used as an
//      unordered_map key. For literal tokens (is_match == false) two
//      tokens are "equal" if their literal byte matches. For match
//      tokens (is_match == true) two tokens are "equal" if their
//      (position, length) pair matches.
//
// Build (example):
//   g++ -std=c++20 test_huffman_count.cpp -o test_huffman_count
//   ./test_huffman_count
//
// No external test framework is used -- everything is driven by
// assert() so this compiles/runs anywhere.

#include <cassert>
#include <iostream>
#include <vector>
#include <sys/types.h>

#include "huffman.hpp"

// ---------------------------------------------------------------------
// Helpers to build fake Token data
// ---------------------------------------------------------------------

// Build a literal (non-match) token carrying a single byte value.
static Token make_literal(unsigned char byte_value) {
    Token t{};
    t.is_match = false;
    t.literal = byte_value;
    return t;
}

// Build a match token with a given (position, length) back-reference.
static Token make_match(u_int16_t position, u_int16_t length) {
    Token t{};
    t.is_match = true;
    t.match.position = position;
    t.match.length = length;
    return t;
}

// Checks whether `tokens` contains an entry matching `expected_token`
// with `expected_count`, regardless of position (unordered_map iteration
// order is not guaranteed, so extract_count's output order isn't either).
static bool contains_token_count(const std::vector<TokenC>& tokens,
                                  const Token& expected_token,
                                  int expected_count) {
    for (const auto& tc : tokens) {
        if (tc.token == expected_token && tc.count == expected_count) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------
// Test cases
// ---------------------------------------------------------------------

// 1. Empty stream should leave byte_counter empty.
static void test_empty_stream() {
    Huffman h;
    std::vector<Token> stream;

    h.count(stream, 0);

    assert(h.byte_counter.empty() && "empty stream should produce empty counter");
    std::cout << "[PASS] test_empty_stream\n";
}

// 2. A single literal token should create exactly one entry with count 1.
static void test_single_literal() {
    Huffman h;
    std::vector<Token> stream = { make_literal('A') };

    h.count(stream, static_cast<int>(stream.size()));

    assert(h.byte_counter.size() == 1);
    auto it = h.byte_counter.find(make_literal('A'));
    assert(it != h.byte_counter.end());
    assert(it->second == 1);
    std::cout << "[PASS] test_single_literal\n";
}

// 3. Repeated literal bytes should accumulate into the same bucket.
static void test_repeated_literal_bytes() {
    Huffman h;
    std::vector<Token> stream = {
        make_literal('B'),
        make_literal('B'),
        make_literal('B'),
        make_literal('C'),
    };

    h.count(stream, static_cast<int>(stream.size()));

    assert(h.byte_counter.size() == 2); // one bucket for 'B', one for 'C'

    auto itB = h.byte_counter.find(make_literal('B'));
    assert(itB != h.byte_counter.end());
    assert(itB->second == 3);

    auto itC = h.byte_counter.find(make_literal('C'));
    assert(itC != h.byte_counter.end());
    assert(itC->second == 1);

    std::cout << "[PASS] test_repeated_literal_bytes\n";
}

// 4. Mixed literal + match tokens should be tracked as distinct keys,
//    even when a match's underlying bits happen to overlap a literal's.
static void test_mixed_literal_and_match_tokens() {
    Huffman h;
    std::vector<Token> stream = {
        make_literal('X'),
        make_match(4, 2),
        make_match(4, 2),   // duplicate match -> should bump same bucket
        make_literal('X'),  // duplicate literal -> should bump same bucket
        make_match(10, 1),  // distinct match -> new bucket
    };

    h.count(stream, static_cast<int>(stream.size()));

    assert(h.byte_counter.size() == 3);

    auto itX = h.byte_counter.find(make_literal('X'));
    assert(itX != h.byte_counter.end());
    assert(itX->second == 2);

    auto itMatch1 = h.byte_counter.find(make_match(4, 2));
    assert(itMatch1 != h.byte_counter.end());
    assert(itMatch1->second == 2);

    auto itMatch2 = h.byte_counter.find(make_match(10, 1));
    assert(itMatch2 != h.byte_counter.end());
    assert(itMatch2->second == 1);

    std::cout << "[PASS] test_mixed_literal_and_match_tokens\n";
}

// 5. STREAM_SIZE smaller than the vector's actual size should only
//    count the first STREAM_SIZE elements, ignoring the rest.
static void test_partial_stream_size() {
    Huffman h;
    std::vector<Token> stream = {
        make_literal('D'),
        make_literal('D'),
        make_literal('E'), // should be ignored, since we only count 2
        make_literal('E'), // should be ignored
    };

    h.count(stream, 2); // only look at the first two elements

    assert(h.byte_counter.size() == 1);
    auto itD = h.byte_counter.find(make_literal('D'));
    assert(itD != h.byte_counter.end());
    assert(itD->second == 2);

    auto itE = h.byte_counter.find(make_literal('E'));
    assert(itE == h.byte_counter.end() && "E tokens beyond STREAM_SIZE must not be counted");

    std::cout << "[PASS] test_partial_stream_size\n";
}

// 6. Calling count() multiple times on the same Huffman instance should
//    accumulate rather than reset the counter.
static void test_accumulates_across_calls() {
    Huffman h;
    std::vector<Token> first_batch  = { make_literal('F'), make_literal('F') };
    std::vector<Token> second_batch = { make_literal('F'), make_literal('G') };

    h.count(first_batch, static_cast<int>(first_batch.size()));
    h.count(second_batch, static_cast<int>(second_batch.size()));

    auto itF = h.byte_counter.find(make_literal('F'));
    assert(itF != h.byte_counter.end());
    assert(itF->second == 3); // 2 from first batch + 1 from second

    auto itG = h.byte_counter.find(make_literal('G'));
    assert(itG != h.byte_counter.end());
    assert(itG->second == 1);

    std::cout << "[PASS] test_accumulates_across_calls\n";
}

// ---------------------------------------------------------------------
// Tests for Huffman::extract_count(std::vector<TokenC>&)
// ---------------------------------------------------------------------

// 7. Empty byte_counter -> extract_count produces an empty vector.
static void test_extract_count_empty() {
    Huffman h;
    std::vector<TokenC> tokens;

    h.extract_count(tokens);

    assert(tokens.empty());
    std::cout << "[PASS] test_extract_count_empty\n";
}

// 8. A single counted token should produce exactly one TokenC with the
//    matching token and count.
static void test_extract_count_single() {
    Huffman h;
    std::vector<Token> stream = { make_literal('A'), make_literal('A') };
    h.count(stream, static_cast<int>(stream.size()));

    std::vector<TokenC> tokens;
    h.extract_count(tokens);

    assert(tokens.size() == 1);
    assert(contains_token_count(tokens, make_literal('A'), 2));

    std::cout << "[PASS] test_extract_count_single\n";
}

// 9. Multiple distinct keys (mix of literal and match tokens) should all
//    show up, with correct counts, regardless of iteration order.
static void test_extract_count_multiple() {
    Huffman h;
    std::vector<Token> stream = {
        make_literal('B'),
        make_literal('B'),
        make_literal('C'),
        make_match(4, 2),
        make_match(4, 2),
        make_match(4, 2),
    };
    h.count(stream, static_cast<int>(stream.size()));

    std::vector<TokenC> tokens;
    h.extract_count(tokens);

    assert(tokens.size() == 3);
    assert(contains_token_count(tokens, make_literal('B'), 2));
    assert(contains_token_count(tokens, make_literal('C'), 1));
    assert(contains_token_count(tokens, make_match(4, 2), 3));

    std::cout << "[PASS] test_extract_count_multiple\n";
}

// 10. extract_count should reflect whatever is currently in byte_counter
//     at call time -- calling it again after more counting should pick
//     up the updated totals.
static void test_extract_count_reflects_latest_state() {
    Huffman h;
    std::vector<Token> first_batch = { make_literal('D') };
    h.count(first_batch, static_cast<int>(first_batch.size()));

    std::vector<TokenC> first_tokens;
    h.extract_count(first_tokens);
    assert(first_tokens.size() == 1);
    assert(contains_token_count(first_tokens, make_literal('D'), 1));

    std::vector<Token> second_batch = { make_literal('D'), make_literal('E') };
    h.count(second_batch, static_cast<int>(second_batch.size()));

    std::vector<TokenC> second_tokens;
    h.extract_count(second_tokens);
    assert(second_tokens.size() == 2);
    assert(contains_token_count(second_tokens, make_literal('D'), 2));
    assert(contains_token_count(second_tokens, make_literal('E'), 1));

    std::cout << "[PASS] test_extract_count_reflects_latest_state\n";
}

// 11. extract_count should not lose or duplicate entries: total count
//     of TokenC entries should equal the number of distinct keys, and
//     the sum of counts should equal the number of tokens originally fed in.
static void test_extract_count_preserves_total_occurrences() {
    Huffman h;
    std::vector<Token> stream = {
        make_literal('F'), make_literal('F'), make_literal('F'),
        make_literal('G'),
        make_match(1, 1), make_match(1, 1),
    };
    h.count(stream, static_cast<int>(stream.size()));

    std::vector<TokenC> tokens;
    h.extract_count(tokens);

    int total = 0;
    for (const auto& tc : tokens) {
        total += tc.count;
    }

    assert(tokens.size() == 3);           // F, G, match(1,1)
    assert(total == static_cast<int>(stream.size())); // 6 tokens fed in total

    std::cout << "[PASS] test_extract_count_preserves_total_occurrences\n";
}

int main() {
    test_empty_stream();
    test_single_literal();
    test_repeated_literal_bytes();
    test_mixed_literal_and_match_tokens();
    test_partial_stream_size();
    test_accumulates_across_calls();

    test_extract_count_empty();
    test_extract_count_single();
    test_extract_count_multiple();
    test_extract_count_reflects_latest_state();
    test_extract_count_preserves_total_occurrences();

    std::cout << "\nAll tests passed.\n";
    return 0;
}