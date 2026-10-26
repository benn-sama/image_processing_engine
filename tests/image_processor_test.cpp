#include "../include/image_processor.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Testing Chunk Validity", "[image][chunks]") {
    Image img("photos/1080_1.ppm");

    std::string dest = "photos/1080_test";
    img.dest(dest);
    img.clone();
    std::vector<std::vector<unsigned char>> test_chunk = img.get_chunk_row();

    // have to do this again so we can point _dst pass the header bytes
    img.dest(dest);
    img.clone();
    std::vector<std::vector<unsigned char>> full_img   = img.return_img();

    CAPTURE(test_chunk[0][0], full_img[0][0]);
    REQUIRE(test_chunk[0][0] == full_img[0][0]);
}