# Define the compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Isrc/include  # Added -Iinclude to specify the include directory

LDFLAGS = -lcurl  # Link against the libcurl library

# Define the source directory and files
SRC_DIR = src
SRC = $(shell find $(SRC_DIR) -name '*.cpp')
TARGET = example

# Default target to build the executable
all: $(TARGET)

# Rule to compile the source files and link them into an executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

# Rule to clean up the build files
clean:
	rm -f $(TARGET)

# Rule to run the executable
run: $(TARGET)
	./$(TARGET)