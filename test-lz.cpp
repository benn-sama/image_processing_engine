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

int main() {
    // LZ77 lz77;
    test_basic_four_bytes();
    test_all_zeros();
    test_all_ones();
    test_known_pattern();
    test_single_byte();
    test_two_bytes();
    test_nonzero_start_pos();
    // lz77.parse();
    // // lz77.printE();
    // lz77.compress();
    // lz77.print();
    return 0;
}