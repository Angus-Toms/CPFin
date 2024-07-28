# Define the build directory
BUILD_DIR = build
TARGET = cpp_finance

# Default target to build the executable
all: $(BUILD_DIR)/$(TARGET)

# Rule to create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to run CMake and build the project
$(BUILD_DIR)/$(TARGET): | $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && $(MAKE)

# Rule to clean up the build files
clean:
	rm -rf $(BUILD_DIR)

# Rule to run the executable
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)
