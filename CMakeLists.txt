cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(cpp_finance VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the fmt and curl packages
find_package(fmt REQUIRED)
find_package(CURL REQUIRED)

# Include directories
include_directories(src/include)

# Add the source directory and files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# Add an executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt CURL::libcurl)

# Optionally add compiler flags (e.g., -Wall, -Wextra, -O2)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)
