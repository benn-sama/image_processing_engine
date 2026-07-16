#include "lz77.hpp"
#include <sys/types.h>
#include <cassert>

void test_basic_four_bytes() {
    std::vector<unsigned char> data = {0x12, 0x34, 0x56, 0x78};
    std::cout << "\n------TESTING FOUR BYTES------\n";
    LZ77 lz;
    assert(lz.package_bytes(data, 0, 4) == 0x12345678);
    std::cout << "\n------TESTING FOUR BYTES FINISHED------\n";
}

void test_all_zeros() {
    std::vector<unsigned char> data = {0x00, 0x00, 0x00, 0x00};
    std::cout << "\n------TESTING FOUR ZERO BYTES------\n";
    LZ77 lz;
    assert(lz.package_bytes(data, 0, 4) == 0x00000000);
    std::cout << "\n------TESTING FOUR ZERO BYTES FINISHED------\n";
}

void test_all_ones() {
    std::vector<unsigned char> data = {0xFF, 0xFF, 0xFF, 0xFF};
    LZ77 lz;
    std::cout << "\n------TESTING FOUR ONE BYTES------\n";
    assert(lz.package_bytes(data, 0, 4) == 0xFFFFFFFF);
    std::cout << "\n------TESTING FOUR ONE BYTES FINISHED------\n";
}

void test_known_pattern() {
    std::vector<unsigned char> data = {0xDE, 0xAD, 0xBE, 0xEF};
    LZ77 lz;
    std::cout << "\n------TESTING KNOWN PATTERNED BYTES------\n";
    assert(lz.package_bytes(data, 0, 4) == 0xDEADBEEF);
    std::cout << "\n------TESTING KNOWN PATTERNED BYTES FINISHED------\n";
}

void test_single_byte() {
    std::vector<unsigned char> data = {0xAB};
    LZ77 lz;
    std::cout << "\n------TESTING SINGLE BYTES------\n";
    assert(lz.package_bytes(data, 0, 1) == 0x000000AB);
    std::cout << "\n------TESTING SINGLE BYTES FINISHED------\n";
}

void test_two_bytes() {
    std::vector<unsigned char> data = {0x01, 0x02};
    LZ77 lz;
    std::cout << "\n------TESTING TWO BYTES------\n";
    assert(lz.package_bytes(data, 0, 2) == 0x00000102);
    std::cout << "\n------TESTING TWO BYTES FINISHED------\n";
}

void test_nonzero_start_pos() {
    std::vector<unsigned char> data = {0xFF, 0xFF, 0x11, 0x22, 0x33, 0x44, 0xFF};
    LZ77 lz;
    std::cout << "\n------TESTING NONZERO START POS BYTES------\n";
    assert(lz.package_bytes(data, 2, 4) == 0x11223344);
    std::cout << "\n------TESTING NONZERO START POS BYTES FINISHED------\n";
}

// Assumes: LZ77 class with public `token_buffer` (std::vector<Token>),
// Token{ bool is_match; unsigned char literal; struct { uint16_t position; uint16_t length; } match; }
// and MAX_WINDOW_SIZE constant accessible/known.

// Helper to convert a string into the byte vector compress() expects.
static std::vector<unsigned char> to_bytes(const std::string& s) {
    return std::vector<unsigned char>(s.begin(), s.end());
}

// Test 1: No repeated 4+ byte sequences -> every byte should be emitted as a literal.
void test_all_literals_when_no_repeats() {
    std::cout << "[STARTING] test_all_literals_when_no_repeats" << std::endl;

    LZ77 lz;
    auto data = to_bytes("abcdefgh"); // no repeating 4-byte windows
    int size = static_cast<int>(data.size());

    lz.compress(data, size);

    assert(lz.token_buffer.size() == data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        assert(lz.token_buffer[i].is_match == false);
        assert(lz.token_buffer[i].literal == data[i]);
    }

    std::cout << "[COMPLETE] test_all_literals_when_no_repeats" << std::endl;
}

// Test 2: A clear repeated sequence should produce a match token.
void test_basic_match_detected() {
    std::cout << "[STARTING] test_basic_match_detected" << std::endl;

    LZ77 lz;
    // "abcd" repeats starting at index 0 and index 6
    auto data = to_bytes("abcdXXabcd");
    int size = static_cast<int>(data.size());

    lz.compress(data, size);

    bool found_match = false;
    for (auto const& t : lz.token_buffer) {
        if (t.is_match) {
            found_match = true;
            assert(t.match.length >= 4);
            assert(t.match.position > 0);
        }
    }
    assert(found_match);

    std::cout << "[COMPLETE] test_basic_match_detected" << std::endl;
}

// Test 3: The tail-end bytes (< 4 remaining) must always be emitted as literals,
// even if they could theoretically match something earlier.
void test_tail_end_forced_literals() {
    std::cout << "[STARTING] test_tail_end_forced_literals" << std::endl;

    LZ77 lz;
    // "abcd" then later only 3 leftover bytes "abc" (matches partially but < 4 remaining forces literal path)
    auto data = to_bytes("abcdXXXabc");
    int size = static_cast<int>(data.size());

    lz.compress(data, size);

    // Reconstruct how many literal tokens correspond to the final 3 bytes
    // by checking the last tokens are literals for 'a','b','c'
    assert(lz.token_buffer.size() >= 3);
    size_t n = lz.token_buffer.size();
    assert(lz.token_buffer[n - 1].is_match == false);
    assert(lz.token_buffer[n - 2].is_match == false);
    assert(lz.token_buffer[n - 3].is_match == false);
    assert(lz.token_buffer[n - 1].literal == 'c');
    assert(lz.token_buffer[n - 2].literal == 'b');
    assert(lz.token_buffer[n - 3].literal == 'a');

    std::cout << "[COMPLETE] test_tail_end_forced_literals" << std::endl;
}

// Test 4: Matches beyond MAX_WINDOW_SIZE distance should NOT be treated as matches.
void test_match_outside_window_size_is_literal() {
    std::cout << "[STARTING] test_match_outside_window_size_is_literal" << std::endl;

    LZ77 lz;
    // Build data where the same 4-byte pattern appears once at the start,
    // then far enough away (beyond MAX_WINDOW_SIZE) to be ineligible for matching.
    std::vector<unsigned char> data;
    auto prefix = to_bytes("abcd");
    data.insert(data.end(), prefix.begin(), prefix.end());

    // Fill with filler bytes exceeding MAX_WINDOW_SIZE distance
    for (int i = 0; i < LZ77::MAX_WINDOW_SIZE + 10; ++i) {
        data.push_back('X');
    }

    auto suffix = to_bytes("abcd");
    data.insert(data.end(), suffix.begin(), suffix.end());

    int size = static_cast<int>(data.size());
    lz.compress(data, size);

    // The second "abcd" occurrence should be emitted as literals (since prior
    // occurrence is outside MAX_WINDOW_SIZE), not as a match.
    size_t n = lz.token_buffer.size();
    assert(lz.token_buffer[n - 1].is_match == false);
    assert(lz.token_buffer[n - 2].is_match == false);
    assert(lz.token_buffer[n - 3].is_match == false);
    assert(lz.token_buffer[n - 4].is_match == false);

    std::cout << "[COMPLETE] test_match_outside_window_size_is_literal" << std::endl;
}

// Test 5: Empty input should produce no tokens at all.
void test_empty_input_produces_no_tokens() {
    std::cout << "[STARTING] test_empty_input_produces_no_tokens" << std::endl;

    LZ77 lz;
    std::vector<unsigned char> data;
    int size = 0;

    lz.compress(data, size);

    assert(lz.token_buffer.empty());

    std::cout << "[COMPLETE] test_empty_input_produces_no_tokens" << std::endl;
}

// Test 6: Repeated long pattern should produce a match with length capped at 65535.
void test_match_length_does_not_exceed_uint16_max() {
    std::cout << "[STARTING] test_match_length_does_not_exceed_uint16_max" << std::endl;

    LZ77 lz;
    // Construct a long run of the same byte to force a very long match.
    std::vector<unsigned char> data(70000, 'a');
    int size = static_cast<int>(data.size());

    lz.compress(data, size);

    bool found_capped_match = false;
    for (auto const& t : lz.token_buffer) {
        if (t.is_match && t.match.length == 258) {
            found_capped_match = true;
        }
        assert(t.match.length <= 258);
    }
    assert(found_capped_match);


    std::cout << "[COMPLETE] test_match_length_does_not_exceed_uint16_max" << std::endl;
}

int main() {
    // LZ77 lz77;
    test_basic_four_bytes();
    test_all_zeros();
    test_all_ones();
    test_known_pattern();
    test_single_byte();
    test_two_bytes();
    test_nonzero_start_pos();
    test_all_literals_when_no_repeats();
    test_basic_four_bytes();
    test_tail_end_forced_literals();
    test_basic_match_detected();
    test_empty_input_produces_no_tokens();
    test_match_length_does_not_exceed_uint16_max();
    // lz77.parse();
    // // lz77.printE();
    // lz77.compress();
    // lz77.print();
    return 0;
}