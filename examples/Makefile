# Define the build directory
BUILD_DIR = build

# List of example executables
EXAMPLES = basic overlays plots timeseries

# Default target: build all examples
all: $(EXAMPLES)

# Rule to create the build directory and configure with CMake
$(BUILD_DIR)/CMakeCache.txt:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..

# Rule to build each example using CMake
$(EXAMPLES): %: $(BUILD_DIR)/CMakeCache.txt
	cd $(BUILD_DIR) && cmake --build . --target $@

# Rules to run each example
run_basic: basic
	./$(BUILD_DIR)/basic

run_overlays: overlays
	./$(BUILD_DIR)/overlays

run_plots: plots
	./$(BUILD_DIR)/plots

run_timeseries: timeseries
	./$(BUILD_DIR)/timeseries

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean $(EXAMPLES) run_basic run_overlays run_plots
