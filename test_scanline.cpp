#include "filter.hpp"
#include "image_processor.hpp"
#include <fstream>
#include <memory>
#include <unistd.h>
#include <assert.h>

void test_subf() {
    Filter sn;

    std::cout << "testing subf()...\n";

// bpp == 0 short-circuit
    if (sn.subf(10, 0) != 0) {
        std::cout << "10:0 is not 0, got " << sn.subf(10, 0) << std::endl;
    }

    // normal positive cases
    if (sn.subf(10, 3) != 7) {
        std::cout << "10:3 is not 7, got " << sn.subf(10, 3) << std::endl;
    }
    if (sn.subf(3, 3) != 0) {
        std::cout << "3:3 is not 0, got " << sn.subf(3, 3) << std::endl;
    }

    // underflow cases (current_byte < bpp) -- expected values assume SIGNED int
    if (sn.subf(0, 1) != -1) {   // unsigned would be 255
        std::cout << "0:1 is not -1, got " << sn.subf(0, 1) << std::endl;
    }
    if (sn.subf(1, 4) != -3) {   // unsigned would be 253
        std::cout << "1:4 is not -3, got " << sn.subf(1, 4) << std::endl;
    }
    if (sn.subf(2, 4) != -2) {   // unsigned would be 254
        std::cout << "2:4 is not -2, got " << sn.subf(2, 4) << std::endl;
    }

    // wrap boundaries
    if (sn.subf(259, 3) != 0) {
        std::cout << "259:3 is not 0, got " << sn.subf(259, 3) << std::endl;
    }
    if (sn.subf(258, 3) != 255) {
        std::cout << "258:3 is not 255, got " << sn.subf(258, 3) << std::endl;
    }
    if (sn.subf(260, 3) != 1) {
        std::cout << "260:3 is not 1, got " << sn.subf(260, 3) << std::endl;
    }

    // large input
    if (sn.subf(1000, 8) != 224) {
        std::cout << "1000:8 is not 248, got " << sn.subf(1000, 8) << std::endl;
    }

    std::cout << "subf finished.\n";
}

void test_sub() {
    Filter sn;
    Image    img;

    std::string str = "./photos/car.ppm"; 
    img.source(str);
    std::fstream* src = img.get_ptr();

    sn.allocate(img.get_width(), img.get_height(), img.get_offset(), src);
    sn.sub();
    sn.print_new_img();
}

void test_Filter() {
    Filter sn;
    Image    img;

    std::string str = "./photos/car.ppm"; 
    img.source(str);
    std::fstream* src = img.get_ptr();

    // img.print();

    std::cout << "running...\n";
    std::cout << "allocating...\n";
    sn.allocate(img.get_width(), img.get_height(), img.get_offset(), src);

    std::cout << "finished allocating.\n";
    std::cout << "verifying...\n";
    std::cout << "offset: " << img.get_offset() << std::endl;
    sn.verify(img.get_width(), img.get_height(), img.get_offset(), src);
    std::cout << "verified.\n";

    // sn.print_new_img();
    // sn.print_old_img();
}

void test_upf() {
    Filter sn;
    Image img;

    std::string str = "./photos/car.ppm";
    img.source(str);
    std::fstream* src = img.get_ptr();

    std::cout << "running...\n";
    std::cout << "allocating...\n";
    sn.allocate(img.get_width(), img.get_height(), img.get_offset(), src);

    std::cout << "testing upf()\n";

    // Test cases for upf() function
    std::cout << "\nTesting upf() function...\n";
    
    // Test 1: current_byte >= prior (no underflow)
    int result1 = sn.upf(100, 50);
    assert(result1 == 50);  // 100 - 50 = 50
    std::cout << "Test 1 passed: upf(100, 50) = " << result1 << " (expected 50)\n";
    
    // Test 2: current_byte < prior (with underflow adjustment)
    int result2 = sn.upf(30, 100);
    assert(result2 == 186);  // 30 - 100 + 256 = 186
    std::cout << "Test 2 passed: upf(30, 100) = " << result2 << " (expected 186)\n";
    
    // Test 3: Equal values
    int result3 = sn.upf(75, 75);
    assert(result3 == 0);  // 75 - 75 = 0
    std::cout << "Test 3 passed: upf(75, 75) = " << result3 << " (expected 0)\n";
    
    // Test 4: Edge case - zero
    int result4 = sn.upf(0, 0);
    assert(result4 == 0);  // 0 - 0 = 0
    std::cout << "Test 4 passed: upf(0, 0) = " << result4 << " (expected 0)\n";
    
    // Test 5: Edge case - maximum byte value
    int result5 = sn.upf(255, 255);
    assert(result5 == 0);  // 255 - 255 = 0
    std::cout << "Test 5 passed: upf(255, 255) = " << result5 << " (expected 0)\n";
    
    // Test 6: current_byte = 0, prior = 255 (maximum underflow)
    int result6 = sn.upf(0, 255);
    assert(result6 == 1);  // 0 - 255 + 256 = 1
    std::cout << "Test 6 passed: upf(0, 255) = " << result6 << " (expected 1)\n";
    
    // Test 7: current_byte = 255, prior = 0 (maximum value)
    int result7 = sn.upf(255, 0);
    assert(result7 == 255);  // 255 - 0 = 255
    std::cout << "Test 7 passed: upf(255, 0) = " << result7 << " (expected 255)\n";
    
    // Test 8: Random case with underflow
    int result8 = sn.upf(50, 200);
    assert(result8 == 106);  // 50 - 200 + 256 = 106
    std::cout << "Test 8 passed: upf(50, 200) = " << result8 << " (expected 106)\n";
    
    std::cout << "\nAll upf() tests passed!\n";
}

void test_up() {
    Filter sn;
    Image img;

    std::string str = "./photos/car.ppm";
    img.source(str);
    std::fstream* src = img.get_ptr();

    std::cout << "running...\n";
    std::cout << "allocating...\n";
    sn.allocate(img.get_width(), img.get_height(), img.get_offset(), src);

    std::cout << "testing up()\n";
    sn.up();
    sn.print_new_img();
}

void test_avgf() {
    Filter sn;
    Image img;

    std::string str = "./photo/hills.ppm";
    img.source(str);
    std::fstream* src = img.get_ptr();

    std::cout << "running...\n";
    std::cout << "allocating...\n";
    sn.allocate(img.get_width(), img.get_height(), img.get_offset(), src);

    // Test cases for avgf() function
    std::cout << "\nTesting avgf() function...\n";
    
    // Test 1: Basic case - positive result
    int result1 = sn.avgf(100, 50, 40);
    // (100 - floor((40 + 50) / 2)) % 256 = (100 - 45) % 256 = 55
    if (result1 != 55) {
        std::cout << "Test 1 FAILED: avgf(100, 50, 40) = " << result1 << " (expected 55)\n";
        assert(false);
    }
    std::cout << "Test 1 passed: avgf(100, 50, 40) = " << result1 << " (expected 55)\n";
    
    // Test 2: Negative result requiring modulo adjustment
    int result2 = sn.avgf(20, 50, 40);
    // (20 - floor((40 + 50) / 2)) % 256 = (20 - 45) % 256 = -25 % 256 = 231
    if (result2 != 231) {
        std::cout << "Test 2 FAILED: avgf(20, 50, 40) = " << result2 << " (expected 231)\n";
        assert(false);
    }
    std::cout << "Test 2 passed: avgf(20, 50, 40) = " << result2 << " (expected 231)\n";
    
    // Test 3: All equal values
    int result3 = sn.avgf(100, 100, 100);
    // (100 - floor((100 + 100) / 2)) % 256 = (100 - 100) % 256 = 0
    if (result3 != 0) {
        std::cout << "Test 3 FAILED: avgf(100, 100, 100) = " << result3 << " (expected 0)\n";
        assert(false);
    }
    std::cout << "Test 3 passed: avgf(100, 100, 100) = " << result3 << " (expected 0)\n";
    
    // Test 4: Edge case - all zeros
    int result4 = sn.avgf(0, 0, 0);
    // (0 - floor((0 + 0) / 2)) % 256 = 0
    if (result4 != 0) {
        std::cout << "Test 4 FAILED: avgf(0, 0, 0) = " << result4 << " (expected 0)\n";
        assert(false);
    }
    std::cout << "Test 4 passed: avgf(0, 0, 0) = " << result4 << " (expected 0)\n";
    
    // Test 5: Edge case - all max values
    int result5 = sn.avgf(255, 255, 255);
    // (255 - floor((255 + 255) / 2)) % 256 = (255 - 255) % 256 = 0
    if (result5 != 0) {
        std::cout << "Test 5 FAILED: avgf(255, 255, 255) = " << result5 << " (expected 0)\n";
        assert(false);
    }
    std::cout << "Test 5 passed: avgf(255, 255, 255) = " << result5 << " (expected 0)\n";
    
    // Test 6: Max current_byte, zero others
    int result6 = sn.avgf(255, 0, 0);
    // (255 - floor((0 + 0) / 2)) % 256 = (255 - 0) % 256 = 255
    if (result6 != 255) {
        std::cout << "Test 6 FAILED: avgf(255, 0, 0) = " << result6 << " (expected 255)\n";
        assert(false);
    }
    std::cout << "Test 6 passed: avgf(255, 0, 0) = " << result6 << " (expected 255)\n";
    
    // Test 7: Zero current_byte, max others
    int result7 = sn.avgf(0, 255, 255);
    // (0 - floor((255 + 255) / 2)) % 256 = (0 - 255) % 256 = -255 % 256 = 1
    if (result7 != 1) {
        std::cout << "Test 7 FAILED: avgf(0, 255, 255) = " << result7 << " (expected 1)\n";
        assert(false);
    }
    std::cout << "Test 7 passed: avgf(0, 255, 255) = " << result7 << " (expected 1)\n";
    
    // Test 8: Odd average requiring floor
    int result8 = sn.avgf(100, 50, 51);
    // (100 - floor((51 + 50) / 2)) % 256 = (100 - floor(50.5)) % 256 = (100 - 50) % 256 = 50
    if (result8 != 50) {
        std::cout << "Test 8 FAILED: avgf(100, 50, 51) = " << result8 << " (expected 50)\n";
        assert(false);
    }
    std::cout << "Test 8 passed: avgf(100, 50, 51) = " << result8 << " (expected 50)\n";
    
    // Test 9: Large go_back_n value
    int result9 = sn.avgf(128, 100, 200);
    // (128 - floor((200 + 100) / 2)) % 256 = (128 - 150) % 256 = -22 % 256 = 234
    if (result9 != 234) {
        std::cout << "Test 9 FAILED: avgf(128, 100, 200) = " << result9 << " (expected 234)\n";
        assert(false);
    }
    std::cout << "Test 9 passed: avgf(128, 100, 200) = " << result9 << " (expected 234)\n";
    
    // Test 10: Small values with negative result
    int result10 = sn.avgf(10, 20, 30);
    // (10 - floor((30 + 20) / 2)) % 256 = (10 - 25) % 256 = -15 % 256 = 241
    if (result10 != 241) {
        std::cout << "Test 10 FAILED: avgf(10, 20, 30) = " << result10 << " (expected 241)\n";
        assert(false);
    }
    std::cout << "Test 10 passed: avgf(10, 20, 30) = " << result10 << " (expected 241)\n";
    
    // Test 11: Mixed values
    int result11 = sn.avgf(150, 100, 50);
    // (150 - floor((50 + 100) / 2)) % 256 = (150 - 75) % 256 = 75
    if (result11 != 75) {
        std::cout << "Test 11 FAILED: avgf(150, 100, 50) = " << result11 << " (expected 75)\n";
        assert(false);
    }
    std::cout << "Test 11 passed: avgf(150, 100, 50) = " << result11 << " (expected 75)\n";
    
    // Test 12: Current byte equals sum average
    int result12 = sn.avgf(75, 50, 100);
    // (75 - floor((100 + 50) / 2)) % 256 = (75 - 75) % 256 = 0
    if (result12 != 0) {
        std::cout << "Test 12 FAILED: avgf(75, 50, 100) = " << result12 << " (expected 0)\n";
        assert(false);
    }
    std::cout << "Test 12 passed: avgf(75, 50, 100) = " << result12 << " (expected 0)\n";
    
    std::cout << "\nAll avgf() tests passed!\n";
}

void test_avg() {
    Filter sn;
    Image img;

    std::string str = "./photos/car.ppm";
    img.source(str);
    std::fstream* src = img.get_ptr();

    std::cout << "running...\n";
    std::cout << "allocating...\n";
    sn.allocate(img.get_width(), img.get_height(), img.get_offset(), src);

    std::cout << "testing avg()\n";
    sn.avg();
    sn.print_new_img();
}

void test_paethf() {
    Filter sn;
    Image img;

    std::string str = "./photo/hills.ppm";
    img.source(str);
    std::fstream* src = img.get_ptr();

    std::cout << "running...\n";
    std::cout << "allocating...\n";
    sn.allocate(img.get_width(), img.get_height(), img.get_offset(), src);

    // Test cases for paethf() function
    std::cout << "\nTesting paethf() function...\n";
    
    // Test 1: All equal values
    int result1 = sn.paethf(100, 100, 100);
    // init_est = 100 + 100 - 100 = 100
    // current_distance = |100 - 100| = 0
    // left_distance = |100 - 100| = 0
    // top_left_distance = |100 - 100| = 0
    // 0 <= 0 (true) && 100 <= 0 (false) -> else if: 0 <= 0 (true) -> left
    if (result1 != 100) {
        std::cout << "Test 1 FAILED: paethf(100, 100, 100) = " << result1 << " (expected 100)\n";
        assert(false);
    }
    std::cout << "Test 1 passed: paethf(100, 100, 100) = " << result1 << " (expected 100)\n";
    
    // Test 2: up distance is smallest
    int result2 = sn.paethf(50, 150, 200);
    // init_est = 50 + 150 - 200 = 0
    // current_distance = |0 - 50| = 50
    // left_distance = |0 - 150| = 150
    // top_left_distance = |0 - 200| = 200
    // 50 <= 150 (true) && 50 <= 200 (true) -> return up
    if (result2 != 50) {
        std::cout << "Test 2 FAILED: paethf(50, 150, 200) = " << result2 << " (expected 50)\n";
        assert(false);
    }
    std::cout << "Test 2 passed: paethf(50, 150, 200) = " << result2 << " (expected 50)\n";
    
    // Test 3: left distance is smallest
    int result3 = sn.paethf(150, 50, 200);
    // init_est = 150 + 50 - 200 = 0
    // current_distance = |0 - 150| = 150
    // left_distance = |0 - 50| = 50
    // top_left_distance = |0 - 200| = 200
    // 150 <= 50 (false) -> else if: 50 <= 200 (true) -> return left
    if (result3 != 50) {
        std::cout << "Test 3 FAILED: paethf(150, 50, 200) = " << result3 << " (expected 50)\n";
        assert(false);
    }
    std::cout << "Test 3 passed: paethf(150, 50, 200) = " << result3 << " (expected 50)\n";
    
    // Test 4: top_left distance is smallest
    int result4 = sn.paethf(200, 200, 50);
    // init_est = 200 + 200 - 50 = 350
    // current_distance = |350 - 200| = 150
    // left_distance = |350 - 200| = 150
    // top_left_distance = |350 - 50| = 300
    // 150 <= 150 (true) && 200 <= 300 (true) -> return up
    if (result4 != 200) {
        std::cout << "Test 4 FAILED: paethf(200, 200, 50) = " << result4 << " (expected 200)\n";
        assert(false);
    }
    std::cout << "Test 4 passed: paethf(200, 200, 50) = " << result4 << " (expected 200)\n";
    
    // Test 5: Zero values
    int result5 = sn.paethf(0, 0, 0);
    // init_est = 0 + 0 - 0 = 0
    // current_distance = |0 - 0| = 0
    // left_distance = |0 - 0| = 0
    // top_left_distance = |0 - 0| = 0
    // 0 <= 0 (true) && 0 <= 0 (true) -> return up
    if (result5 != 0) {
        std::cout << "Test 5 FAILED: paethf(0, 0, 0) = " << result5 << " (expected 0)\n";
        assert(false);
    }
    std::cout << "Test 5 passed: paethf(0, 0, 0) = " << result5 << " (expected 0)\n";
    
    // Test 6: Max values
    int result6 = sn.paethf(255, 255, 255);
    // init_est = 255 + 255 - 255 = 255
    // current_distance = |255 - 255| = 0
    // left_distance = |255 - 255| = 0
    // top_left_distance = |255 - 255| = 0
    // 0 <= 0 (true) && 255 <= 0 (false) -> else if: 0 <= 0 (true) -> return left
    if (result6 != 255) {
        std::cout << "Test 6 FAILED: paethf(255, 255, 255) = " << result6 << " (expected 255)\n";
        assert(false);
    }
    std::cout << "Test 6 passed: paethf(255, 255, 255) = " << result6 << " (expected 255)\n";
    
    // Test 7: up = left, top_left different (smaller)
    int result7 = sn.paethf(100, 100, 50);
    // init_est = 100 + 100 - 50 = 150
    // current_distance = |150 - 100| = 50
    // left_distance = |150 - 100| = 50
    // top_left_distance = |150 - 50| = 100
    // 50 <= 50 (true) && 100 <= 100 (true) -> return up
    if (result7 != 100) {
        std::cout << "Test 7 FAILED: paethf(100, 100, 50) = " << result7 << " (expected 100)\n";
        assert(false);
    }
    std::cout << "Test 7 passed: paethf(100, 100, 50) = " << result7 << " (expected 100)\n";
    
    // Test 8: up = left, top_left different (larger)
    int result8 = sn.paethf(50, 50, 100);
    // init_est = 50 + 50 - 100 = 0
    // current_distance = |0 - 50| = 50
    // left_distance = |0 - 50| = 50
    // top_left_distance = |0 - 100| = 100
    // 50 <= 50 (true) && 50 <= 100 (true) -> return up
    if (result8 != 50) {
        std::cout << "Test 8 FAILED: paethf(50, 50, 100) = " << result8 << " (expected 50)\n";
        assert(false);
    }
    std::cout << "Test 8 passed: paethf(50, 50, 100) = " << result8 << " (expected 50)\n";
    
    // Test 9: Different values - left wins
    int result9 = sn.paethf(100, 80, 150);
    // init_est = 100 + 80 - 150 = 30
    // current_distance = |30 - 100| = 70
    // left_distance = |30 - 80| = 50
    // top_left_distance = |30 - 150| = 120
    // 70 <= 50 (false) -> else if: 50 <= 120 (true) -> return left
    if (result9 != 80) {
        std::cout << "Test 9 FAILED: paethf(100, 80, 150) = " << result9 << " (expected 80)\n";
        assert(false);
    }
    std::cout << "Test 9 passed: paethf(100, 80, 150) = " << result9 << " (expected 80)\n";
    
    // Test 10: Larger difference between parameters
    int result10 = sn.paethf(10, 20, 30);
    // init_est = 10 + 20 - 30 = 0
    // current_distance = |0 - 10| = 10
    // left_distance = |0 - 20| = 20
    // top_left_distance = |0 - 30| = 30
    // 10 <= 20 (true) && 10 <= 30 (true) -> return up
    if (result10 != 10) {
        std::cout << "Test 10 FAILED: paethf(10, 20, 30) = " << result10 << " (expected 10)\n";
        assert(false);
    }
    std::cout << "Test 10 passed: paethf(10, 20, 30) = " << result10 << " (expected 10)\n";
    
    // Test 11: Diagonal values
    int result11 = sn.paethf(75, 125, 100);
    // init_est = 75 + 125 - 100 = 100
    // current_distance = |100 - 75| = 25
    // left_distance = |100 - 125| = 25
    // top_left_distance = |100 - 100| = 0
    // 25 <= 25 (true) && 75 <= 0 (false) -> else if: 25 <= 0 (false) -> return top_left
    if (result11 != 100) {
        std::cout << "Test 11 FAILED: paethf(75, 125, 100) = " << result11 << " (expected 100)\n";
        assert(false);
    }
    std::cout << "Test 11 passed: paethf(75, 125, 100) = " << result11 << " (expected 100)\n";
    
    // Test 12: High values with varied distances
    int result12 = sn.paethf(200, 220, 210);
    // init_est = 200 + 220 - 210 = 210
    // current_distance = |210 - 200| = 10
    // left_distance = |210 - 220| = 10
    // top_left_distance = |210 - 210| = 0
    // 10 <= 10 (true) && 200 <= 0 (false) -> else if: 10 <= 0 (false) -> return top_left
    if (result12 != 210) {
        std::cout << "Test 12 FAILED: paethf(200, 220, 210) = " << result12 << " (expected 210)\n";
        assert(false);
    }
    std::cout << "Test 12 passed: paethf(200, 220, 210) = " << result12 << " (expected 210)\n";
    
    std::cout << "\nAll paethf() tests passed!\n";
}

void test_paeth() {
    Filter sn;
    Image img;

    std::string str = "./photos/car.ppm";
    img.source(str);
    std::fstream* src = img.get_ptr();

    std::cout << "running...\n";
    std::cout << "allocating...\n";
    sn.allocate(img.get_width(), img.get_height(), img.get_offset(), src);

    std::cout << "testing paeth()\n";
    sn.paeth();
    sn.print_new_img();
}

int main() {
    test_Filter();
    
    // test sub function and algorithm
    // test_sub();
    // test_subf();

    // test up function and algorithm
    // test_upf();
    // test_up();
    // test_avgf();
    // test_avg();
    // test_paethf();
    // test_paeth();

    return 0;
}