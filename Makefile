CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

# test-lz.cpp and test_scanline.cpp removed — they have their own main() and are built separately
SRC := main.cpp thread.cpp thread_pool.cpp random_num_gen.cpp image_processor.cpp lz77.cpp filter.cpp greyscale.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := main

.PHONY: all run-main run-lztest run-sltest clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp thread.hpp thread_pool.hpp image_processor.hpp lz77.hpp greyscale.hpp filter.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run-main: $(TARGET)
	./$(TARGET)

# Link the test against the LZ77 implementation it depends on
test-lz: test-lz.o lz77.o
	$(CXX) $(CXXFLAGS) -o $@ $^

run-lztest: test-lz
	./test-lz

# Scanline test depends on scanline + image_processor + greyscale
test_scanline.o: test_scanline.cpp filter.hpp image_processor.hpp greyscale.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test-scanline: test_scanline.o filter.o image_processor.o greyscale.o
	$(CXX) $(CXXFLAGS) -o $@ $^

run-sltest: test-scanline
	./test-scanline

clean:
	rm -f $(TARGET) $(OBJ) test-lz test-lz.o test-scanline test_scanline.o