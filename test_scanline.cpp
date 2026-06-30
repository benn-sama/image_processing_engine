#include "filter.hpp"
#include <iostream>
#include <cassert>

int main() {
    Filter filter;
    int pass_count = 0;
    int fail_count = 0;
    
    std::cout << "\n╔════════════════════════════════════════════════╗\n";
    std::cout << "║         FILTER CLASS TEST SUITE                ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n\n";

    // ========== TEST SUBF ==========
    std::cout << "Testing subf()...\n";
    std::cout << "─────────────────────────────────────────\n";
    
    auto test_case = [&](const char* desc, int result, int expected) {
        if (result == expected) {
            std::cout << "  ✓ PASS: " << desc << " = " << result << "\n";
            pass_count++;
        } else {
            std::cout << "  ✗ FAIL: " << desc << " = " << result << " (expected " << expected << ")\n";
            fail_count++;
        }
    };
    
    test_case("subf(10, 0) short-circuit", filter.subf(10, 0), 10);
    test_case("subf(10, 3)", filter.subf(10, 3), 7);
    test_case("subf(3, 3)", filter.subf(3, 3), 0);
    test_case("subf(0, 1) underflow", filter.subf(0, 1), 255);  // -1 mod 256 = 255
    test_case("subf(258, 3) wrap", filter.subf(258, 3), 255);
    test_case("subf(1000, 8)", filter.subf(1000, 8), 224);
    
    // ========== TEST UPF ==========
    std::cout << "\nTesting upf()...\n";
    std::cout << "─────────────────────────────────────────\n";
    
    test_case("upf(100, 50)", filter.upf(100, 50), 50);
    test_case("upf(30, 100) underflow", filter.upf(30, 100), 186);
    test_case("upf(75, 75)", filter.upf(75, 75), 0);
    test_case("upf(0, 0)", filter.upf(0, 0), 0);
    test_case("upf(255, 255)", filter.upf(255, 255), 0);
    test_case("upf(0, 255) max underflow", filter.upf(0, 255), 1);
    test_case("upf(255, 0)", filter.upf(255, 0), 255);
    test_case("upf(50, 200)", filter.upf(50, 200), 106);
    
    // ========== TEST AVGF ==========
    std::cout << "\nTesting avgf()...\n";
    std::cout << "─────────────────────────────────────────\n";
    
    test_case("avgf(100, 50, 40)", filter.avgf(100, 50, 40), 55);
    test_case("avgf(20, 50, 40) negative", filter.avgf(20, 50, 40), 231);
    test_case("avgf(100, 100, 100)", filter.avgf(100, 100, 100), 0);
    test_case("avgf(0, 0, 0)", filter.avgf(0, 0, 0), 0);
    test_case("avgf(255, 255, 255)", filter.avgf(255, 255, 255), 0);
    test_case("avgf(255, 0, 0)", filter.avgf(255, 0, 0), 255);
    test_case("avgf(0, 255, 255)", filter.avgf(0, 255, 255), 1);
    test_case("avgf(100, 50, 51) floor", filter.avgf(100, 50, 51), 50);
    test_case("avgf(128, 100, 200)", filter.avgf(128, 100, 200), 234);
    test_case("avgf(10, 20, 30)", filter.avgf(10, 20, 30), 241);
    test_case("avgf(150, 100, 50)", filter.avgf(150, 100, 50), 75);
    test_case("avgf(75, 50, 100)", filter.avgf(75, 50, 100), 0);
    
    // ========== TEST PAETHF ==========
    std::cout << "\nTesting paethf()...\n";
    std::cout << "─────────────────────────────────────────\n";
    
    test_case("paethf(100, 100, 100) equal", filter.paethf(100, 100, 100), 100);
    test_case("paethf(50, 150, 200) up wins", filter.paethf(50, 150, 200), 50);
    test_case("paethf(150, 50, 200) left wins", filter.paethf(150, 50, 200), 50);
    test_case("paethf(200, 200, 50)", filter.paethf(200, 200, 50), 200);
    test_case("paethf(0, 0, 0)", filter.paethf(0, 0, 0), 0);
    test_case("paethf(255, 255, 255)", filter.paethf(255, 255, 255), 255);
    test_case("paethf(100, 100, 50)", filter.paethf(100, 100, 50), 100);
    test_case("paethf(50, 50, 100)", filter.paethf(50, 50, 100), 50);
    test_case("paethf(100, 80, 150) left", filter.paethf(100, 80, 150), 80);
    test_case("paethf(10, 20, 30)", filter.paethf(10, 20, 30), 10);
    test_case("paethf(75, 125, 100)", filter.paethf(75, 125, 100), 100);
    test_case("paethf(200, 220, 210)", filter.paethf(200, 220, 210), 210);
    
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