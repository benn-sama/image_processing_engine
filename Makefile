CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

SRC := thread.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := main

.PHONY: all run-main clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp thread.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run-main: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)