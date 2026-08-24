CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

SRC_FILES  := filter.o greyscale.o huffman.o image_processor.o lz77.o main.o pngt.o random_num_gen.o thread_pool.o thread.o
TEST_FILES := test_huffman.o test_scanline.o test-lz.o test.o

all: $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $^ -o main

test: $(TEST_FILES) $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $^ -o tests

run-main: all
	./main

run-tests: test
	./tests

$(SRC_FILES): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(TEST_FILES): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm -f *.o main tests

.PHONY: run-main