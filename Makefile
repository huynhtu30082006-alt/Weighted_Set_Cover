# Compiler
CXX = g++

# Flags
CXXFLAGS = -std=c++17 -Wall -Wextra -I./source

# Source files
SRC = $(wildcard source/*.cpp)

# Output
TARGET = app

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Run program
run: $(TARGET)
	./$(TARGET)

# Run with Python visualization
run_graph: $(TARGET)
	./$(TARGET)
	python graph/graph.py

# Clean build
clean:
	rm -f $(TARGET)