#include "lz77.hpp"

int main() {
    LZ77 lz77;

    lz77.parse();
    // lz77.printE();
    lz77.compress();
    // lz77.print();
    return 0;
}