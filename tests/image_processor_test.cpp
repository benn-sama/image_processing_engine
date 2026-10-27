#include "../include/image_processor.hpp"
#include "catch2/catch_message.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cerrno>

TEST_CASE("Testing Chunk Validity", "[image][chunks]") {
    std::string src = "photos/1080_1.ppm";
    Image img(src);

    std::string dest = "photos/1080_test";
    img.dest(dest);
    img.clone();
    std::vector<std::vector<unsigned char>> test_chunk = img.get_chunk_row();

    std::string dest1 = "photos/1080_testt";
    img.source(src);
    img.clone();
    std::vector<std::vector<unsigned char>> full_img   = img.return_img();
    std::cout << "here\n";
   
    // *************** Test if width size is 2400********************
    int expected = 2400;
    int actual   = test_chunk[0].size();

    CAPTURE(expected, actual);
    REQUIRE(expected == actual);

    // *************** Test if first byte, first row is the same as the full img ********************
    expected = full_img[0][0];
    actual   = test_chunk[0][0];

    CAPTURE(expected, actual);
    REQUIRE(expected == actual);

    // *************** Test if last byte, first row is the same as the full img ********************
    expected = full_img[0][full_img[0].size() - 1];
    actual   = test_chunk[0][test_chunk[0].size() - 1];

    CAPTURE(expected, actual);
    REQUIRE(expected == actual);
}