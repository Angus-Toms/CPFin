cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(cpp_finance VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Python 3
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

# Find NumPy
execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
include_directories(${NUMPY_INCLUDE_DIR})

# Add the fmt and curl packages
find_package(fmt REQUIRED)
find_package(CURL REQUIRED)
find_package(Matplot++ REQUIRED)

# Include directories
include_directories(src/include)

# Add the source directory and files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# Add an executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt CURL::libcurl)
target_link_libraries(${PROJECT_NAME} PRIVATE Matplot++::matplot Python3::Python)

# Optionally add compiler flags (e.g., -Wall, -Wextra, -O2)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)
