#include "filter.hpp"
#include <iostream>
#include <cassert>
#include <vector>

// This suite covers the vector-based scanline methods that test_scanline.cpp
// does not exercise: sub(), up(), avg(), paeth(), none(), and filter_scanline().
// Run it alongside test_scanline.cpp (which already covers subf/upf/avgf/paethf).
// TEST WRITTEN BY AI
// MODFIED BY MYSELF


int main() {
    int pass_count = 0;
    int fail_count = 0;

    std::cout << "\n╔════════════════════════════════════════════════╗\n";
    std::cout << "║      FILTER SCANLINE METHODS TEST SUITE        ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n\n";

    auto test_case = [&](const char* desc, int result, int expected) {
        if (result == expected) {
            std::cout << "  ✓ PASS: " << desc << " = " << result << "\n";
            pass_count++;
        } else {
            std::cout << "  ✗ FAIL: " << desc << " = " << result << " (expected " << expected << ")\n";
            fail_count++;
        }
    };

    auto test_row = [&](const char* desc, std::vector<unsigned char>& row,
                         std::vector<unsigned char> const& expected, int limit) {
        bool ok = true;
        for (int i = 0; i < limit; ++i) {
            if (row[i] != expected[i]) { ok = false; break; }
        }
        if (ok) {
            std::cout << "  ✓ PASS: " << desc << " (row matches)\n";
            pass_count++;
        } else {
            std::cout << "  ✗ FAIL: " << desc << " (row mismatch) got: ";
            for (int i = 0; i < limit; ++i) std::cout << (int)row[i] << " ";
            std::cout << " expected: ";
            for (int i = 0; i < limit; ++i) std::cout << (int)expected[i] << " ";
            std::cout << "\n";
            fail_count++;
        }
    };

    // ========== TEST SUB ==========
    std::cout << "Testing sub()...\n";
    std::cout << "─────────────────────────────────────────\n";
    {
        // bpp = (3 channels * 8 bits) / 8 = 3
        Filter filter(3, 8);
        int ARR_SIZE = 7;
        std::vector<unsigned char> bottom = {0, 10, 20, 30, 40, 50, 60};
        std::vector<std::vector<unsigned char>> filtersv(5, std::vector<unsigned char>(ARR_SIZE, 0));

        int avg = filter.sub(bottom, filtersv, ARR_SIZE);
        test_case("sub() average, bpp=3", avg, 21);

        std::vector<unsigned char> expected = {0, 10, 20, 30, 30, 30, 30};
        test_row("sub() row, bpp=3 (short-circuit then wrap-around left byte)", filtersv[1], expected, ARR_SIZE);
    }
    {
        // bpp = (1 channel * 8 bits) / 8 = 1
        Filter filter(1, 8);
        int ARR_SIZE = 5;
        std::vector<unsigned char> bottom = {0, 100, 150, 200, 250};
        std::vector<std::vector<unsigned char>> filtersv(5, std::vector<unsigned char>(ARR_SIZE, 0));

        int avg = filter.sub(bottom, filtersv, ARR_SIZE);
        test_case("sub() average, bpp=1", avg, 50);

        std::vector<unsigned char> expected = {0, 100, 50, 50, 50};
        test_row("sub() row, bpp=1", filtersv[1], expected, ARR_SIZE);
    }

    // ========== TEST UP ==========
    std::cout << "\nTesting up()...\n";
    std::cout << "─────────────────────────────────────────\n";
    {
        Filter filter(3, 8);
        int ARR_SIZE = 5;
        std::vector<unsigned char> top    = {0, 5, 10, 250, 255};
        std::vector<unsigned char> bottom = {0, 8, 10, 5,   0};
        std::vector<std::vector<unsigned char>> filtersv(5, std::vector<unsigned char>(ARR_SIZE, 0));

        int avg = filter.up(bottom, top, filtersv, ARR_SIZE);
        test_case("up() average (includes underflow wrap)", avg, 3);

        std::vector<unsigned char> expected = {0, 3, 0, 11, 1};
        test_row("up() row", filtersv[2], expected, ARR_SIZE);
    }

    // ========== TEST AVG ==========
    std::cout << "\nTesting avg()...\n";
    std::cout << "─────────────────────────────────────────\n";
    {
        Filter filter(3, 8);
        int ARR_SIZE = 6;
        std::vector<unsigned char> top    = {0, 40, 50, 60, 100, 200};
        std::vector<unsigned char> bottom = {0, 100, 50, 30, 20, 10};
        std::vector<std::vector<unsigned char>> filtersv(5, std::vector<unsigned char>(ARR_SIZE, 0));

        int avg = filter.avg(bottom, top, filtersv, ARR_SIZE);
        test_case("avg() average", avg, 95);

        std::vector<unsigned char> expected = {0, 80, 25, 226, 176, 66};
        test_row("avg() row (includes underflow wraps)", filtersv[3], expected, ARR_SIZE);
    }

    // ========== TEST PAETH ==========
    std::cout << "\nTesting paeth()...\n";
    std::cout << "─────────────────────────────────────────\n";
    {
        Filter filter(3, 8);
        int ARR_SIZE = 6;
        std::vector<unsigned char> top    = {0, 10, 20, 30, 40, 50};
        std::vector<unsigned char> bottom = {0, 15, 25, 35, 45, 55};
        std::vector<std::vector<unsigned char>> filtersv(5, std::vector<unsigned char>(ARR_SIZE, 0));

        int avg = filter.paeth(bottom, top, filtersv, ARR_SIZE);
        test_case("paeth() average", avg, 3);

        std::vector<unsigned char> expected = {0, 5, 15, 0, 0, 0};
        test_row("paeth() row", filtersv[4], expected, ARR_SIZE);
    }

    // ========== TEST NONE ==========
    // Row 0 is the "none" filter's row (0=none, 1=sub, 2=up, 3=avg,
    // 4=paeth, matching the tag ordering used throughout). The none
    // filter is an identity transform, so filtersv[0][i] should equal
    // bottom[i], and its comparison sum should be the sum of those raw
    // bytes. These tests assert that correct contract, not whatever the
    // current implementation happens to compute.
    std::cout << "\nTesting none()...\n";
    std::cout << "─────────────────────────────────────────\n";
    {
        Filter filter(3, 8);
        int ARR_SIZE = 5;
        std::vector<unsigned char> bottom = {0, 11, 22, 33, 44};
        std::vector<std::vector<unsigned char>> filtersv(5, std::vector<unsigned char>(ARR_SIZE, 0));

        int avg = filter.none(bottom, filtersv, ARR_SIZE);
        test_case("none() average = sum(bottom[1..n-1]) / ARR_SIZE", avg, 22);

        std::vector<unsigned char> expected = {0, 11, 22, 33, 44};
        test_row("none() writes identity copy of bottom into filtersv[0]", filtersv[0], expected, ARR_SIZE);
    }
    {
        Filter filter(3, 8);
        int ARR_SIZE = 5;
        std::vector<unsigned char> bottom = {0, 5, 10, 15, 20};
        std::vector<std::vector<unsigned char>> filtersv(5, std::vector<unsigned char>(ARR_SIZE, 0));

        int avg = filter.none(bottom, filtersv, ARR_SIZE);
        test_case("none() average, second data set", avg, 10);

        std::vector<unsigned char> expected = {0, 5, 10, 15, 20};
        test_row("none() writes identity copy of bottom into filtersv[0], second data set", filtersv[0], expected, ARR_SIZE);
    }

    // ========== TEST FILTER_SCANLINE ==========
    std::cout << "\nTesting filter_scanline()...\n";
    std::cout << "─────────────────────────────────────────\n";
    // filter_scanline() internally builds its scratch buffer sized to
    // (ARR_SIZE + 1) rows of only 5 (then 6, after a push_back) columns
    // each, but sub()/up()/avg()/paeth()/none() index that buffer as
    // [filter_type][byte_position], i.e. they need ARR_SIZE+ columns per
    // row. That only lines up by coincidence when ARR_SIZE <= 6; for any
    // wider scanline (e.g. anything resembling a real image row) it's a
    // heap out-of-bounds write. Confirmed with ASan: ARR_SIZE=10 aborts
    // with a heap-buffer-overflow inside filter_scanline. These tests are
    // kept to ARR_SIZE <= 6 to actually exercise the selection logic
    // instead of crashing the whole suite.
    //
    // Expected tags below are the correct winner given each filter's true
    // per-byte sum (0=none, 1=sub, 2=up, 3=avg, 4=paeth) -- i.e. assuming
    // none() correctly sums bottom[] as derived above, not the sum of 0
    // that the current buggy none() always contributes. If none() is
    // fixed to match its intended contract, this is the tag
    // filter_scanline should select for each scenario.
    {
        Filter filter(3, 8);
        int ARR_SIZE = 6;
        std::vector<unsigned char> top    = {0, 10, 10, 10, 10, 10};
        std::vector<unsigned char> bottom = {0, 10, 10, 10, 10, 10};
        std::vector<unsigned char> alter(ARR_SIZE + 1, 0);

        filter.filter_scanline(top, bottom, alter, ARR_SIZE);
        // True per-filter sums here: none=8, sub=5, up=0, avg=1, paeth=1.
        // up (tag 2) is the correct winner for an unchanged scanline.
        test_case("filter_scanline() selects up (tag 2) for an unchanged scanline", (int)alter[0], 2);
    }
    {
        Filter filter(3, 8);
        int ARR_SIZE = 6;
        std::vector<unsigned char> top    = {0, 0, 0, 0, 0, 0};
        std::vector<unsigned char> bottom = {0, 200, 200, 200, 200, 200};
        std::vector<unsigned char> alter(ARR_SIZE + 1, 0);

        filter.filter_scanline(top, bottom, alter, ARR_SIZE);
        // True per-filter sums here: none=166, sub=100, up=166, avg=166,
        // paeth=66. paeth (tag 4) is the correct winner for this jump.
        test_case("filter_scanline() selects paeth (tag 4) for a large scanline delta", (int)alter[0], 4);
    }

    // ========== SUMMARY ==========
    std::cout << "\n╔════════════════════════════════════════════════╗\n";
    std::cout << "║                 TEST SUMMARY                   ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n\n";

    int total = pass_count + fail_count;
    std::cout << "Total Tests:  " << total << "\n";
    std::cout << "Passed:       " << pass_count << " ✓\n";
    std::cout << "Failed:       " << fail_count << " ✗\n";
    std::cout << "Success Rate: " << (total > 0 ? (pass_count * 100) / total : 0) << "%\n\n";

    if (fail_count == 0) {
        std::cout << "╔════════════════════════════════════════════════╗\n";
        std::cout << "║          ✓ ALL TESTS PASSED!                   ║\n";
        std::cout << "╚════════════════════════════════════════════════╝\n\n";
        return 0;
    } else {
        std::cout << "╔════════════════════════════════════════════════╗\n";
        std::cout << "║          ✗ SOME TESTS FAILED                   ║\n";
        std::cout << "╚════════════════════════════════════════════════╝\n\n";
        return 1;
    }
}