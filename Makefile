# Define the build directory for the main project
BUILD_DIR = build
TARGET = cpp_finance

# Define the build directory for the tests
TEST_DIR = test
TEST_BUILD_DIR = $(TEST_DIR)/build
TEST_TARGET = mytests

# Default target to build the executable and tests
all: $(BUILD_DIR)/$(TARGET)

# Rule to create the main build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to create the test build directory
$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

# Rule to run CMake and build the main project
$(BUILD_DIR)/$(TARGET): | $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && $(MAKE)

# Rule to build the tests
build_tests: $(TEST_BUILD_DIR)
	cd $(TEST_DIR) && cmake -S . -B build && cmake --build build

# Rule to run the tests
test: build_tests
	cd $(TEST_BUILD_DIR) && ctest --output-on-failure

# Rule to clean up the build files
clean:
	rm -rf $(BUILD_DIR) $(TEST_BUILD_DIR)

# Rule to run the main executable
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

.PHONY: all build_tests test clean run