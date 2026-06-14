CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

SRC = main.cpp
TARGET = file_manager

.PHONY: all clean performance-test correctness-test

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o

performance-test: all
	@echo "Running performance test..."
	@./$(TARGET) performance_test

correctness-test: all
	@echo "Running correctness test..."
	@./$(TARGET) correctness_test

run: all
	./$(TARGET)
