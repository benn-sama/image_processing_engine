#include <fstream>
#include <iostream>

int main() {
    std::fstream* f = new std::fstream("photos/car-p.png", std::ios::in | std::ios::binary);

    // f->seekg(8, std::ios::end);
    // std::streamsize size = f->tellg();
    // f->seekg(0, std::ios::end);

    f->seekg(0, std::ios::beg);
    char* buffer = new char[32];
    f->read(buffer, 32);

    for (int i = 0; i < 32; ++i) {
        std::cout << ' ' << (static_cast<unsigned int>(buffer[i]) & 0xFF) << ' ';
    }

    std::cout << std::endl;
}