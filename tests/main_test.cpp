#include "image_processor_test.cpp"
#include "performance_test.cpp"
#include "test_huffman.cpp"
// #include "test_scanline.cpp"
// #include "test-lz.cpp"
#include "test.cpp"

// --- Forward declarations of your test functions ---
void test_get_chunk();          // used inside image_processor_test
void image_processor_test();
void performance_test();
void test_huffman();
void test();

// --- Dispatcher ---
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0]
                   << " [all|image_processor_test|performance_test|test_huffman|test]\n";
        return 1;
    }

    std::string arg = argv[1];

    // Map of name -> function to call
    std::unordered_map<std::string, std::function<void()>> tests = {
        {"image_processor_test", image_processor_test},
        {"performance_test",     performance_test},
        {"test_huffman",         test_huffman},
        {"test",                 test}
    };

    if (arg == "all") {
        for (auto& [name, fn] : tests) {
            std::cout << "=== Running " << name << " ===\n";
            fn();
        }
        return 0;
    }

    auto it = tests.find(arg);
    if (it == tests.end()) {
        std::cerr << "Unknown argument: " << arg << "\n";
        std::cerr << "Valid options: all, image_processor_test, performance_test, test_huffman, test\n";
        return 1;
    }

    it->second();
    return 0;
}