CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

SRC := main.cpp thread.cpp thread_pool.cpp random_num_gen.cpp image_processor.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := main

.PHONY: all run-main clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp thread.hpp thread_pool.hpp image_processor.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run-main: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)