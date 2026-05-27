#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sys/types.h>
#include <vector>
#include <iomanip>
#include <cstring>
#include <bit>
#include <array>
#include <cstring>

void printHex(const std::string& label, const std::vector<unsigned char>& vec) {
    std::cout << label;
    for (unsigned char byte : vec) {
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)byte << ' ';
    }
    std::cout << std::endl;
}
void printHex(const std::string& label, const std::array<unsigned char, 4>& vec) {
    std::cout << label;
    for (unsigned char byte : vec) {
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)byte << ' ';
    }
    std::cout << std::endl;
}

void printHex(const std::string& label, unsigned char byte) {
    std::cout << label
                << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)byte
                << std::endl;
}

// 33 bytes in total
struct pngHeader {
    std::vector<unsigned char> sig;        // 8 bytes: PNG signature

    // IHDR
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

    // iCCP chunks
    std::vector<unsigned char> iccpLength;       // 4 bytes
    std::vector<unsigned char> iccpChunkType;    // 4 bytes
    std::vector<unsigned char> iccpProfileChunk; // 1-79 bytes
    unsigned char              iccpNull;         // 1 byte
    unsigned char              iccpCompressionM; // 1 byte
    std::vector<unsigned char> iccpCompressionP; // iccpLength - used = bytes: used = summation of the iccp chunks - iccpLength
    std::vector<unsigned char> iccpCRC;         // 4 bytes


    // IDAT chunk
    std::vector<unsigned char> IDATLength; // this includes the summation of zlibheader, deflate data, and adler-32
    std::vector<unsigned char> IDATSig;
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
 
        for (int i = 29; i < 33; ++i) CRC.push_back((unsigned char)arr[i]);
    }
    
    void printDecimal(const std::string& label, unsigned char byte) {
        std::cout << label << static_cast<int>(byte) << std::endl;
    }

    void printDecimal(const std::string& label, const std::vector<unsigned char>& vec) {
        std::array<unsigned char, 4> decVec;
        

        // copy values from vec to decVec
        std::memcpy(decVec.data(), vec.data(), 4);

        // convert into bits
        uint32_t raw = std::bit_cast<uint32_t>(decVec);

        // big-endian
        uint32_t decNum = ((raw >> 24) & 0xFF)       |
                 ((raw >> 8)  & 0xFF00)     |
                 ((raw << 8)  & 0xFF0000)   |
                 ((raw << 24) & 0xFF000000);

        std::cout << label << std::dec << decNum << std::endl;
    }

    void printIHDR() {
        std::cout << "----------------IHDR hex--------------------------" << std::endl;
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

    void printIHDRDec() {
        std::cout << "----------------IHDR decimal--------------------------" << std::endl;
        printDecimal("PNG signature:   ", sig);
        printDecimal("IHDR length:     ", IDHRLength);
        printDecimal("chunk type:      ", chunkType);
        printDecimal("IHDR - width:           ", width);
        printDecimal("IHDR - height:          ", height);
        printDecimal("IHDR - bit depth:       ", bitDepth);  // number of bits per channel
        printDecimal("IHDR - color:           ", color);      // 0 = palette (PLTE), 1 = color, 2 = alpha present
        printDecimal("IHDR - compress method: ", compressM);
        printDecimal("IHDR - filter:          ", filter);
        printDecimal("IHDR - interlace:       ", interlace);
        printDecimal("CRC:             ", CRC);
    }
    
    void IccpChunk(std::vector<char>& arr) {
        // iccp length
        for (int i = 33; i < 37; ++i) {
            iccpLength.push_back(arr[i]);
        }

        // from bytes to a useful number
        std::array<unsigned char, 4> number;
        std::memcpy(number.data(), iccpLength.data(), 4);
        uint32_t raw = std::bit_cast<uint32_t>(number);
        uint32_t length = ((raw >> 24) & 0xFF)       |
                 ((raw >> 8)  & 0xFF00)     |
                 ((raw << 8)  & 0xFF0000)   |
                 ((raw << 24) & 0xFF000000);

        // iccp chunk type
        for (int i = 37; i < 41; ++i) {
            iccpChunkType.push_back(arr[i]);
        }

        // iccpProfileChunk here (needs to read until it hits a null terminator (00 or 0x00))
        int count = 41;
        while (arr[count] != 0) {
            iccpProfileChunk.push_back(arr[count]);
            ++count;
        }

        ++count;
        iccpNull = arr[count];

        // get the remaining count
        uint32_t indexStart = 37;
        uint32_t remaining = length - (count - indexStart);

        // iccpCompressionP here (needs to calc: length - used)
        // this includes the current count
        count++;
        for (int i = count; i < remaining + count; ++i) {
            iccpCompressionP.push_back(arr[i]);
        }

        /// check if it is correct
        if (uint32_t(iccpCompressionP.size()) == remaining) {
            std::cout << "compressionP size is equal to the iccpLength" << std::endl;
        } else {
            std::cout << "compressionP size: " << iccpCompressionP.size() + count << std::endl;
            std::cout << "remaining size: " << remaining << std::endl;
        }

        // CRC (needs to get from where iccpCompressionP leaves off)
        // including
        count += remaining;
        for (int i = count; i < count + 4; ++i) {
            iccpCRC.push_back(arr[i]);
        }
    }

    void IccpPrint() {
        std::cout << "----------------iCCp hex--------------------------" << std::endl;
        printHex("iCCP length: ", iccpLength);
        printHex("iCCP chunk type:   ", iccpChunkType);
        printHex("iCCP profile chunk: ", iccpProfileChunk);
        printHex("iCCP null: ", iccpNull);
        printHex("iCCP compression method: ", iccpCompressionM);
        printHex("iCCP compression profile: ", iccpCompressionP);
        printHex("iCCP CRC: ", iccpCRC);
    }

    void IccpPrintDec() {
        std::cout << "----------------iCCp decimal--------------------------" << std::endl;
        printDecimal("iCCP length: ", iccpLength);
        printDecimal("iCCP chunk type:   ", iccpChunkType);
        printDecimal("iCCP profile chunk: ", iccpProfileChunk);
        printDecimal("iCCP null: ", iccpNull);
        printDecimal("iCCP compression method: ", iccpCompressionM);
        printDecimal("iCCP compression profile: ", iccpCompressionP);
        printDecimal("iCCP CRC: ", iccpCRC);
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

    void printIccp() {
        
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


// IDAT chunks
struct IDATChunk {
    std::array<unsigned char, 4> length;
    std::array<unsigned char, 4> chunkType;
    std::vector<unsigned char> data;
    std::array<unsigned char, 4> CRC;
    uint32_t lengthD = 0;

    void print() {
        std::cout << "----------------IDAT hex--------------------------" << std::endl;
        printHex("IDAT length: ", length);
        printHex("IDAT chunk type (identifier):   ", chunkType);
        printHex("IDAT data: ", data);
        printHex("IDAT CRC: ", CRC);
        std::cout << "IDAT Length (decimal): " << lengthD << std::endl;
    }
};

uint32_t to_decimal(std::array<unsigned char, 4> IDATLengthHex) {
        uint32_t raw = std::bit_cast<uint32_t>(IDATLengthHex);
        uint32_t length = ((raw >> 24) & 0xFF)       |
                 ((raw >> 8)  & 0xFF00)     |
                 ((raw << 8)  & 0xFF0000)   |
                 ((raw << 24) & 0xFF000000);

        return length;
}

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

    // int startIndex = 0;
    // for (int i = 0; i < int(buffer.size()) - 4; ++i) {
    //     if (std::memcmp(buffer.data() + i, "IDAT", 4) == 0) {
    //         startIndex = i;
    //         std::cout << "StartIndex: " << startIndex << std::endl;
    //         break;
    //     }
    // }

    std::vector<IDATChunk> chunks;
    IDATChunk temp;
    int currIndex = 0;

    std::cout << "Looking for IDAT chunks\n" << std::endl;
    while (std::memcmp(buffer.data() + currIndex, "IEND", 4) != 0) {
        // stopped here
        if (std::memcmp(buffer.data() + currIndex, "IDAT", 4) == 0) {
            std::cout << "Found IDAT chunk @: " << currIndex << std::endl;
            std::array<unsigned char, 4> hLength = {
                static_cast<unsigned char>(buffer[currIndex - 4]),
                static_cast<unsigned char>(buffer[currIndex - 3]), 
                static_cast<unsigned char>(buffer[currIndex - 2]), 
                static_cast<unsigned char>(buffer[currIndex - 1])
            };

            uint32_t dLength = to_decimal(hLength);

            std::array<unsigned char, 4> chunkT = {
                static_cast<unsigned char>(buffer[currIndex]),
                static_cast<unsigned char>(buffer[currIndex + 1]),
                static_cast<unsigned char>(buffer[currIndex + 2]),
                static_cast<unsigned char>(buffer[currIndex + 3])
            };

            std::vector<unsigned char> data;
            for (int j = currIndex + 4; j < dLength + currIndex; ++j) {
                data.push_back(buffer[j]);
            }
            currIndex += dLength;

            std::array<unsigned char, 4> CRC = {
                static_cast<unsigned char>(buffer[currIndex]),
                static_cast<unsigned char>(buffer[currIndex + 1]),
                static_cast<unsigned char>(buffer[currIndex + 2]),
                static_cast<unsigned char>(buffer[currIndex + 3])
            };

            chunks.emplace_back(hLength, chunkT, data, CRC, dLength);
        }
        ++currIndex;
    }

    std::cout << "Found IEND @: " << currIndex << std::endl;


}
    

    
    // pngHeader header(buffer);
    // header.printIHDR();
    // header.printIHDRDec();
    // header.IccpChunk(buffer);
    // header.IccpPrint();
    // header.IccpPrintDec();

    // header.IDATChunk(buffer);
    // header.printIDAT();

    // for (int i = 0; i < 32; ++i) {
    //     std::cout << ' ' << (static_cast<unsigned int>(buffer[i]) & 0xFF) << ' ';
    // }