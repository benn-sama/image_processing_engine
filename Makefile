CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

SRC_FILES    		:= filter.o greyscale.o huffman.o image_processor.o lz77.o main.o pngt.o random_num_gen.o ./thread_pool/thread_pool.o ./thread_pool/thread.o
TEST_FILES   	    := tests/test_huffman.o tests/test_scanline.o  tests/test.o
TEST_FILES_COMPILED := $(TEST_FILES:.o=)
COMPILED_FILES      := /tests/test_huffman tests/test tests/test_scanline

# compiles everything, assuming no tests
all: $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $^ -o main

# compiles everything INCLUDING tests
tests: $(SRC_FILES) $(TEST_FILES) $(TEST_FILES_COMPILED)

run-main: all
	./main

$(SRC_FILES): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(TEST_FILES): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(TEST_FILES_COMPILED): tests/%: tests/%.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# these are to reference the .o
tests/test_huffman: huffman.o
tests/test_scanline: filter.o
# tests/test-lz: lz77.o

# obviously this is to clean up nasty compiled files
clean:
	rm -f *.o main test & \
	rm -f ./thread_pool/*.o & \
	rm -f ./tests/test ./tests/test_scanline  ./tests/test_huffman \
	rm -f ./tests/*.o

.PHONY: run-main all tests