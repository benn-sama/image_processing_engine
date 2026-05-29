CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

# test-lz.cpp removed — it has its own main() and is built separately
SRC := main.cpp thread.cpp thread_pool.cpp random_num_gen.cpp image_processor.cpp lz77.cpp greyscale.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := main

.PHONY: all run-main run-lztest clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp thread.hpp thread_pool.hpp image_processor.hpp lz77.hpp greyscale.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run-main: $(TARGET)
	./$(TARGET)

# Link the test against the LZ77 implementation it depends on
test-lz: test-lz.o lz77.o
	$(CXX) $(CXXFLAGS) -o $@ $^

run-lztest: test-lz
	./test-lz

clean:
	rm -f $(TARGET) $(OBJ) test-lz test-lz.o