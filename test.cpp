#include <fstream>
#include <iostream>
#include <vector>

// 33 bytes in total
struct pngHeader {
    std::vector<char> sig        = std::vector<char>(4);
    std::vector<char> IDHRLength = std::vector<char>(4);
    std::vector<char> chunkType  = std::vector<char>(4);
    std::vector<char> width      = std::vector<char>(4);
    std::vector<char> height     = std::vector<char>(4);
    char              bitDepth   = '\0'; // number of bits per channel
    char              color      = '\0'; // 0 = palette (PLTE), 1 = color, 2 = alpha present
    char              compressM  = '\0';
    char              filter     = '\0';
    char              interlace  = '\0';
    std::vector<char> CRC        = std::vector<char>(4);  // this is padded (takes up more space)

    pngHeader(std::vector<char>& arr) {
        
    }
};

int main() {
    std::fstream* f = new std::fstream("photos/car-p.png", std::ios::in | std::ios::binary);

    // f->seekg(8, std::ios::end);
    // std::streamsize size = f->tellg();
    // f->seekg(0, std::ios::end);

    // 29 bytes in the header
    f->seekg(0, std::ios::beg);
    char* buffer = new char[32];
    f->read(buffer, 32);

    std::vector<char> 

    // for (int i = 0; i < 32; ++i) {
    //     std::cout << ' ' << (static_cast<unsigned int>(buffer[i]) & 0xFF) << ' ';
    // }

    std::cout << std::endl;
}