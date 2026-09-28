#include "../image_processor.hpp"

void test_get_chunk() {
    Image img("photos/1080_1.ppm");

    std::string dest = "photos/1080_test";
    img.dest(dest);

    img.clone();
    img.get_chunk();
}

void image_processor_test() {
    test_get_chunk();
}