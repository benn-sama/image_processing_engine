CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -w

SRC_FILES    		:= filter.o greyscale.o huffman.o image_processor.o lz77.o main.o pngt.o random_num_gen.o ./thread_pool/thread_pool.o ./thread_pool/thread.o
TEST_FILES   	    := tests/test_huffman.o tests/test_scanline.o  tests/test.o tests/image_processor_test.o tests/performance_test.o
MAIN_TEST_FILE      := tests/main_test.o
COMPILED_FILES      := /tests/test_huffman tests/test tests/test_scanline 


# compiles everything except tests
all: $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ main

run-main: all
	./main

run-test-all: main_test
	./main_test all

run-test-image_processor: main_test
	./main_test image_processor_test

run-test-performance: main_test
	./main_test performance_test

run-test-huffman: main_test
	./main_test test_huffman

run-test-test: main_test
	./main_test test

main_test: $(MAIN_TEST_FILE) $(filter-out main.o,$(SRC_FILES)) $(TEST_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_FILES): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(MAIN_TEST_FILE): tests/main_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_FILES): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

# these are to reference the .o
tests/test_huffman: huffman.o
tests/test_scanline: filter.o
# tests/test-lz: lz77.o

# obviously this is to clean up nasty compiled files
clean:
	rm -f *.o main test & \
	rm -f ./thread_pool/*.o & \
	rm -f ./tests/*.o ./tests/main_test \
	rm -f -- photos/*copy*

.PHONY: run-main run-test-all run-test-image_processor run-test-performance run-test-huffman run-test-test all clean