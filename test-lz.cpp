#include "lz77.hpp"
#include <sys/types.h>
#include <cassert>

void test_basic_four_bytes() {
    std::vector<unsigned char> data = {0x12, 0x34, 0x56, 0x78};
    assert(package_bytes(data, 0, 4) == 0x12345678);
}

void test_all_zeros() {
    std::vector<unsigned char> data = {0x00, 0x00, 0x00, 0x00};
    assert(package_bytes(data, 0, 4) == 0x00000000);
}

void test_all_ones() {
    std::vector<unsigned char> data = {0xFF, 0xFF, 0xFF, 0xFF};
    assert(package_bytes(data, 0, 4) == 0xFFFFFFFF);
}

void test_known_pattern() {
    std::vector<unsigned char> data = {0xDE, 0xAD, 0xBE, 0xEF};
    assert(package_bytes(data, 0, 4) == 0xDEADBEEF);
}

void test_single_byte() {
    std::vector<unsigned char> data = {0xAB};
    assert(package_bytes(data, 0, 1) == 0x000000AB);
}

void test_two_bytes() {
    std::vector<unsigned char> data = {0x01, 0x02};
    assert(package_bytes(data, 0, 2) == 0x00000102);
}

void test_nonzero_start_pos() {
    std::vector<unsigned char> data = {0xFF, 0xFF, 0x11, 0x22, 0x33, 0x44, 0xFF};
    assert(package_bytes(data, 2, 4) == 0x11223344);
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