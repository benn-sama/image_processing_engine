#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

// 33 bytes in total
struct pngHeader {
    std::vector<unsigned char> sig;        // 8 bytes: PNG signature
    std::vector<unsigned char> IDHRLength; // 4 bytes: IHDR/chunk length
    std::vector<unsigned char> chunkType;  // 4 bytes: chunk type
    std::vector<unsigned char> width;      // 4 bytes: width
    std::vector<unsigned char> height;     // 4 bytes: height
    unsigned char bitDepth  = 0; // 1 byte: number of bits per channel (either 1, 2, 4, 8, or 16)
    unsigned char color     = 0; // 1 byte: 0 = palette (PLTE), 1 = color, 2 = alpha present (0, 2, 3, 4, 7)
    unsigned char compressM = 0; // 1 byte: deflate
    unsigned char filter    = 0; // 1 byte
    unsigned char interlace = 0; // 1 byte
    std::vector<unsigned char> CRC; // 4 bytes: this is padded (takes up more space)

    // IDAT chunk
    std::vector<unsigned char> IDATLength;
    std::vector<unsigned char> IDAT;
    std::vector<unsigned char> zlibHeader;
    std::vector<unsigned char> DEFLATEdata;
    std::vector<unsigned char> adlerChecksum;
    std::vector<unsigned char> IDATCRC; 

    pngHeader(std::vector<char>& arr) {
        for (int i = 0; i < 8;   ++i) sig.push_back((unsigned char)arr[i]);
        for (int i = 8; i < 12;   ++i) IDHRLength.push_back((unsigned char)arr[i]);
        for (int i = 12; i < 16;  ++i) chunkType.push_back((unsigned char)arr[i]);
        for (int i = 16; i < 20; ++i) width.push_back((unsigned char)arr[i]);
        for (int i = 20; i < 24; ++i) height.push_back((unsigned char)arr[i]);

        bitDepth  = (unsigned char)arr[24];
        color     = (unsigned char)arr[25];
        compressM = (unsigned char)arr[26];
        filter    = (unsigned char)arr[27];
        interlace = (unsigned char)arr[28];
 
        for (int i = 28; i < 32; ++i) CRC.push_back((unsigned char)arr[i]);
    }

    void printHex(const std::string& label, const std::vector<unsigned char>& vec) {
        std::cout << label;
        for (unsigned char byte : vec)
            std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)byte << ' ';
        std::cout << std::endl;
    }

    void printHex(const std::string& label, unsigned char byte) {
        std::cout << label
                  << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)byte
                  << std::endl;
    }

    void printIHDR() {
        printHex("PNG signature:   ", sig);
        printHex("IHDR length:     ", IDHRLength);
        printHex("chunk type:      ", chunkType);
        printHex("IHDR - width:           ", width);
        printHex("IHDR - height:          ", height);
        printHex("IHDR - bit depth:       ", bitDepth);  // number of bits per channel
        printHex("IHDR - color:           ", color);      // 0 = palette (PLTE), 1 = color, 2 = alpha present
        printHex("IHDR - compress method: ", compressM);
        printHex("IHDR - filter:          ", filter);
        printHex("IHDR - interlace:       ", interlace);
        printHex("CRC:             ", CRC);
    }

    void IDATChunk(std::vector<char>& arr) {
        for (int i = 32; i < 36; ++i) {
            IDATLength.push_back((unsigned char)arr[i]);
        }

        for (int i = 36; i < 40; ++i) {
            IDAT.push_back((unsigned char)arr[i]);
        }

        for (int i = 40; i < 42; ++i) {
            zlibHeader.push_back(arr[i]);
        }
    }

    void printIDAT() {
        printHex("IDAT Length: ", IDATLength);
        printHex("IDAT:        ", IDAT);
        printHex("zlib header  ", zlibHeader);
        // deflate data
        // adler-32 checksum
        // IDAT crc
    }
};

int main() {
    std::fstream* f = new std::fstream("photos/car-p.png", std::ios::in | std::ios::binary);

    f->seekg(8, std::ios::end);
    std::streamsize size = f->tellg();
    f->seekg(0, std::ios::end);

    // 29 bytes in the header
    f->seekg(0, std::ios::beg);
    // char* buffer = new char[32];
    std::vector<char> buffer = std::vector<char>(size);
    f->read(buffer.data(), size);

    pngHeader header(buffer);
    header.printIHDR();
    std::cout << "-------------------------------------------------------------------------\n";
    header.IDATChunk(buffer);
    header.printIDAT();

    // for (int i = 0; i < 32; ++i) {
    //     std::cout << ' ' << (static_cast<unsigned int>(buffer[i]) & 0xFF) << ' ';
    // }
}