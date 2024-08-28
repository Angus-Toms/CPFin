cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Examples)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add any required packages
find_package(fmt REQUIRED)
find_package(CURL REQUIRED)
find_package(Python3 COMPONENTS Development NumPy)

# Include directories for your project's headers and external libraries
include_directories(../src/include)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})

# Find all source files in the examples directory
set(EXAMPLE_FILES
    basic.cpp
    overlays.cpp
    plots.cpp
)

set(SRC_FILES
    ../src/priceseries.cpp
    ../src/print_utils.cpp
    ../src/time_utils.cpp
    ../src/overlays/bollinger.cpp
    ../src/overlays/ema.cpp
    ../src/overlays/sma.cpp
    ../src/overlays/macd.cpp
    ../src/overlays/rsi.cpp
)

# Add an executable for each example source file
foreach(FILE ${EXAMPLE_FILES})
    get_filename_component(EXAMPLE_NAME ${FILE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${FILE} ${SRC_FILES})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE fmt::fmt CURL::libcurl ${Python3_LIBRARIES})
    target_compile_options(${EXAMPLE_NAME} PRIVATE -Wall -Wextra -O2)
endforeach()
