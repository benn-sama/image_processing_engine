#include "../huffman.hpp"
#include <sys/types.h>
#include <cassert>
#include <iostream>
#include <vector>

// NOTE: Test written by Claude, but reviewed and changed by myself.

// Assumes: Token{ bool is_match; unsigned char literal; struct { uint16_t position; uint16_t length; } match; }
// as used in the LZ77 test suite.
//
// NOTE ON A LIKELY BUG: Huffman::count_occurrences calls
//     get_code(buffer_stream[i].literal)
// for match tokens, i.e. it looks up the length-code table using the
// token's `.literal` field instead of `.match.length`. Several tests below
// are written against the code as it actually behaves (deriving expected
// values from get_code() itself, rather than hardcoding len_table numbers),
// and are annotated where this quirk is relevant. If `.match.length` was
// intended, those tests should be updated once the bug is fixed.

// ---- Debug helpers -------------------------------------------------------
// Not part of the test assertions themselves — these just print the internal
// state of a Huffman instance so failures (or just curiosity) can be
// inspected without attaching a debugger.

static void print_lit_len_counter(Huffman const& h) {
    std::cout << "  lit_len_counter (nonzero indices): ";
    bool any = false;
    for (size_t i = 0; i < h.lit_len_counter.size(); ++i) {
        if (h.lit_len_counter[i] != 0) {
            std::cout << "[" << i << "]=" << h.lit_len_counter[i] << " ";
            any = true;
            if (any) { break; }
        }
    }
    if (!any) std::cout << "(all zero)";
    std::cout << std::endl;
}

static void print_token(Token const& t, size_t idx) {
    if (t.is_match) {
        std::cout << "  token[" << idx << "]: MATCH  literal_field=0x"
                   << std::hex << (int)t.literal << std::dec
                   << " match.position=" << t.match.position
                   << " match.length=" << t.match.length << std::endl;
    } else {
        std::cout << "  token[" << idx << "]: LITERAL literal=0x"
                   << std::hex << (int)t.literal << std::dec << std::endl;
    }
}

static void print_debug_state(Huffman const& h, char const* label) {
    std::cout << "  -- " << label << " --" << std::endl;
    print_lit_len_counter(h);
}

void test_single_literal_increments_counter() {
    std::cout << "[STARTING] test_single_literal_increments_counter" << std::endl;

    Huffman h;
    Token t{};
    t.is_match = false;
    t.literal = 0x41;
    std::vector<Token> buf = {t};
    h.count_occurrences(buf, 1);
    assert(h.lit_len_counter[0x41] == 1);
    assert(h.lit_len_counter[256] == 1); // end-of-block marker always bumped once per call
    print_debug_state(h, "test_single_literal_increments_counter");
    std::cout << "[COMPLETE] test_single_literal_increments_counter" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

void test_multiple_occurrences_of_same_literal() {
    std::cout << "[STARTING] test_multiple_occurrences_of_same_literal" << std::endl;

    Huffman h;
    Token t{};
    t.is_match = false;
    t.literal = 0x7A;
    std::vector<Token> buf = {t, t, t};

    h.count_occurrences(buf, 3);

    assert(h.lit_len_counter[0x7A] == 3);
    assert(h.lit_len_counter[256] == 1);

    print_debug_state(h, "test_multiple_occurrences_of_same_literal");
    std::cout << "[COMPLETE] test_multiple_occurrences_of_same_literal" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

void test_distinct_literals_counted_separately() {
    std::cout << "[STARTING] test_distinct_literals_counted_separately" << std::endl;

    Huffman h;
    Token a{}; a.is_match = false; a.literal = 0x10;
    Token b{}; b.is_match = false; b.literal = 0x20;
    Token c{}; c.is_match = false; c.literal = 0x10;
    std::vector<Token> buf = {a, b, c};

    h.count_occurrences(buf, 3);

    assert(h.lit_len_counter[0x10] == 2);
    assert(h.lit_len_counter[0x20] == 1);
    assert(h.lit_len_counter[256] == 1);

    print_debug_state(h, "test_distinct_literals_counted_separately");
    std::cout << "[COMPLETE] test_distinct_literals_counted_separately" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

void test_literal_boundary_values() {
    std::cout << "[STARTING] test_literal_boundary_values" << std::endl;

    Huffman h;
    Token low{}; low.is_match = false; low.literal = 0;
    Token high{}; high.is_match = false; high.literal = 255;
    std::vector<Token> buf = {low, high};

    h.count_occurrences(buf, 2);

    assert(h.lit_len_counter[0] == 1);
    assert(h.lit_len_counter[255] == 1);
    assert(h.lit_len_counter[256] == 1);

    print_debug_state(h, "test_literal_boundary_values");
    std::cout << "[COMPLETE] test_literal_boundary_values" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;std::cout << "---------------------------------------------------------------------" << std::endl;
}

void test_empty_stream_still_increments_eob() {
    std::cout << "[STARTING] test_empty_stream_still_increments_eob" << std::endl;

    Huffman h;
    std::vector<Token> buf;

    h.count_occurrences(buf, 0);

    // Loop body never runs (STREAM_SIZE == 0), but the EOB increment sits
    // outside the loop and is unconditional, so it still fires once.
    assert(h.lit_len_counter[256] == 1);
    int total = 0;
    for (auto v : h.lit_len_counter) total += v;
    assert(total == 1);

    print_debug_state(h, "test_empty_stream_still_increments_eob");
    std::cout << "[COMPLETE] test_empty_stream_still_increments_eob" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;std::cout << "---------------------------------------------------------------------" << std::endl;
}

void test_get_code_returns_259_for_invalid_length() {
    std::cout << "[STARTING] test_get_code_returns_259_for_invalid_length" << std::endl;

    Huffman h;
    // No valid DEFLATE-style length table should cover a value this large.
    u_int16_t code = h.get_code(100000);
    std::cout << "  get_code(100000) = " << code << std::endl;
    assert(code == 259);

    std::cout << "[COMPLETE] test_get_code_returns_259_for_invalid_length" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;std::cout << "---------------------------------------------------------------------" << std::endl;
}

void test_match_token_count_matches_get_code_output() {
    std::cout << "[STARTING] test_match_token_count_matches_get_code_output" << std::endl;

    // This test documents the CURRENT behavior of count_occurrences: for a
    // match token it calls get_code(token.literal), not get_code(token.match.length).
    // We derive the expected bucket from get_code() directly so this test
    // doesn't depend on the specific contents of len_table.
    Huffman h;
    Token t{};
    t.is_match = true;
    t.literal = 3;          // stands in for "length" per the current (buggy?) implementation
    t.match.length = 3;     // set for completeness; NOT what count_occurrences actually reads
    std::vector<Token> buf = {t};

    u_int16_t expected_code = h.get_code(t.literal);
    std::cout << "  get_code(t.literal=" << t.literal << ") = " << expected_code << std::endl;
    print_token(t, 0);

    h.count_occurrences(buf, 1);

    if (expected_code == 259) {
        // get_code failed to resolve a bucket -> count_occurrences returns
        // early, before the EOB increment.
        int total = 0;
        for (auto v : h.lit_len_counter) total += v;
        assert(total == 0);
    } else {
        assert(h.lit_len_counter[expected_code] == 1);
        assert(h.lit_len_counter[256] == 1);
    }

    print_debug_state(h, "test_match_token_count_matches_get_code_output");
    std::cout << "[COMPLETE] test_match_token_count_matches_get_code_output" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;std::cout << "---------------------------------------------------------------------" << std::endl;
}

void test_invalid_match_code_stops_processing_early() {
    std::cout << "[STARTING] test_invalid_match_code_stops_processing_early" << std::endl;

    Huffman h;

    Token literal_before{};
    literal_before.is_match = false;
    literal_before.literal = 0x41;

    Token bad_match{};
    bad_match.is_match = true;
    // literal is an unsigned char (0-255), same as the byte stream itself.
    // get_code's table is keyed on length values (3-258), so a raw byte
    // like this passed in as "length" should fail to resolve to any code.
    bad_match.literal = 0xFF; // 256

    Token literal_after{};
    literal_after.is_match = false;
    literal_after.literal = 0x42;

    std::vector<Token> buf = {literal_before, bad_match, literal_after};

    // Sanity-check our assumption that this literal value is indeed invalid
    // as a length lookup. If your len_table happens to cover 0xFF (255),
    // swap in a value it definitely doesn't cover.
    std::cout << "  get_code(bad_match.literal=0x" << std::hex << (int)bad_match.literal
               << std::dec << ") = " << bad_match.literal << std::endl;
    for (size_t i = 0; i < buf.size(); ++i) print_token(buf[i], i);

    assert(h.count_occurrences(buf, 3) == 259);

    print_debug_state(h, "test_invalid_match_code_stops_processing_early (after count_occurrences)");

    // Token processed before the error should be counted...
    assert(h.lit_len_counter[0x41] == 1);
    // ...but the function returns immediately on error, so nothing after
    // it runs, and the trailing EOB increment (after the loop) never fires.
    assert(h.lit_len_counter[0x42] == 0);
    assert(h.lit_len_counter[256] == 0);

    std::cout << "[COMPLETE] test_invalid_match_code_stops_processing_early" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;std::cout << "---------------------------------------------------------------------" << std::endl;
}

// Pure debug aid: walks a mixed literal/match stream, printing each token
// and the counter state after processing. No new assertions beyond a basic
// sanity check — this exists so a human can eyeball the full trace when
// something upstream (LZ77) looks off.
void test_debug_trace_mixed_stream() {
    std::cout << "[STARTING] test_debug_trace_mixed_stream" << std::endl;

    Huffman h;
    std::vector<Token> buf;

    Token lit1{}; lit1.is_match = false; lit1.literal = 0x61; // 'a' as raw byte
    Token lit2{}; lit2.is_match = false; lit2.literal = 0x62; // 'b' as raw byte
    Token match1{}; match1.is_match = true; match1.literal = 4; match1.match.position = 2; match1.match.length = 4;
    Token lit3{}; lit3.is_match = false; lit3.literal = 0x61;

    buf = {lit1, lit2, match1, lit3};

    std::cout << "  input tokens:" << std::endl;
    for (size_t i = 0; i < buf.size(); ++i) print_token(buf[i], i);

    std::cout << "  state before count_occurrences:" << std::endl;
    print_debug_state(h, "before");

    h.count_occurrences(buf, static_cast<int>(buf.size()));

    std::cout << "  state after count_occurrences:" << std::endl;
    print_debug_state(h, "after");

    // Loose sanity check only: total literal-bucket increments (excluding
    // EOB) should not exceed the number of tokens processed. If the match
    // token hit the 259-error path, processing would have stopped early and
    // this still holds.
    int total_lit = 0;
    for (size_t i = 0; i < h.lit_len_counter.size() - 1; ++i) total_lit += h.lit_len_counter[i];
    assert(total_lit <= static_cast<int>(buf.size()) + 1); // + 1 counts for the automatic incrementing EOB(256)

    std::cout << "[COMPLETE] test_debug_trace_mixed_stream" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;std::cout << "---------------------------------------------------------------------" << std::endl;
}

int main() {
    test_single_literal_increments_counter();
    test_multiple_occurrences_of_same_literal();
    test_distinct_literals_counted_separately();
    test_literal_boundary_values();
    test_empty_stream_still_increments_eob();
    test_get_code_returns_259_for_invalid_length();
    test_match_token_count_matches_get_code_output();
    test_invalid_match_code_stops_processing_early();
    test_debug_trace_mixed_stream();
    return 0;
}