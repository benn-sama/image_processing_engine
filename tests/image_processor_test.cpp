#include "../include/image_processor.hpp"
#include "catch2/catch_message.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cerrno>

// global variables

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fstream>
#include <stdexcept>
#include <string>

// Creates a binary PPM (P6) image that is 3 rows x 30 bytes per row.
// 30 bytes per row = 10 RGB pixels (3 bytes each), so the image is 10 wide x 3 tall.
// Every pixel in a given row has the same (R, G, B) values:
//   Row 0: R=124 (0x7C), G=100 (0x64), B=50  (0x32)
//   Row 1: R=200 (0xC8), G=30  (0x1E), B=90  (0x5A)
//   Row 2: R=15  (0x0F), G=180 (0xB4), B=240 (0xF0)
void create_test_ppm(std::string file_name) {
    const int rows = 3;
    const int pixels_per_row = 10;          // 10 pixels * 3 bytes = 30 bytes per row
    const unsigned char row_rgb[rows][3] = {
        {124, 100,  50},
        {200,  30,  90},
        { 15, 180, 240}
    };

    std::ofstream out(file_name, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Could not open file: " + file_name);
    }

    // PPM header: magic number, width, height, max color value
    out << "P6\n" << pixels_per_row << " " << rows << "\n255\n";

    // Pixel data: each row repeats the same R, G, B triple 10 times
    for (int r = 0; r < rows; ++r) {
        for (int p = 0; p < pixels_per_row; ++p) {
            out.put(static_cast<char>(row_rgb[r][0]));  // R
            out.put(static_cast<char>(row_rgb[r][1]));  // G
            out.put(static_cast<char>(row_rgb[r][2]));  // B
        }
    }

    std::cout << file_name << " has been created.\n";
}

TEST_CASE("Testing Chunk Data Validity", "[image][chunks]") {
    std::string src;
    Image       img;
    std::string dst1;
    std::string dst2;
    
    src = "photos/1080_1.ppm";
    dst1 = "photos/1080_test";
    dst2 = "photos/1080_testt";

    img.source(src) ;
    img.dest(dst1);
    img.clone();
    std::vector<std::vector<unsigned char>> chunk = img.get_chunk_row();

    img.source(src);
    img.dest(dst2);
    img.clone();
    std::vector<std::vector<unsigned char>> full_img   = img.return_img();

    // store these important mf stats
    int chunk_width  = chunk[0].size();
    int chunk_height = chunk.size();
    int img_width    = full_img[0].size();
    int img_height   = full_img.size();
   
    // *************** Test if width size is 2400********************
    int expected = 2400;
    int actual   = img_width;

    CAPTURE(expected, actual);
    REQUIRE(expected == actual);

    expected = 160;
    actual  = chunk_height;

    CAPTURE(expected, actual);
    REQUIRE(expected == actual);

    // *************** Test if first byte, first row is the same as the full img ********************
    expected = full_img[0][0];
    actual   = chunk[0][0];

    CAPTURE(expected, actual);
    REQUIRE(expected == actual);

    // *************** Test if last byte, first row is the same as the full img ********************
    expected = full_img[0][chunk_width - 1];
    actual   = chunk[0][img_width - 1];

    CAPTURE(expected, actual);
    REQUIRE(expected == actual);

    // *************** checks if the chunk is byte by byte the same with the full img ********************
    for (int i = 0; i < chunk_height; ++i) {
        for (int j = 0; j < chunk_width; ++j) {
            expected = chunk[i][j];
            actual   = full_img[i][j];

            CAPTURE(expected, actual);
            REQUIRE(expected == actual);
        }
    }
}

TEST_CASE("Testing Grayscale Row Chunks", "[image][chunks][grayscale][sthread]") {
    create_test_ppm("photos/3x30_test.ppm");
    Image img;
    img.strat_A();
    std::vector<std::vector<unsigned char>> gray_img = img.return_whole_chunk();
    
    const int height = 3;
    const int width  = 30;
    const unsigned char row_bytes[height] = {0x65, 0x46, 0x95};

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            unsigned char expected = row_bytes[i];
            unsigned char actual   = gray_img[i][j];

            CAPTURE(expected, actual);
            REQUIRE(expected == actual);
        }
    }
}