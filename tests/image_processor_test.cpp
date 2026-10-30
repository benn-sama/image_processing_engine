#include "../include/image_processor.hpp"
#include "catch2/catch_message.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cerrno>

// global variables

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

    // *************** checks if the queue is chronologically correct ********************
    img.fill_queue();
    for (int i = 0; i < 160; ++i) {
        expected = i;
        actual   = img.pop_front();
        CAPTURE(expected, actual);
        REQUIRE(expected == actual);
    }
}