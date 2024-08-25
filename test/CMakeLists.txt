cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(cpp_finance_tests VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

# Add the fmt and curl packages
find_package(fmt REQUIRED)
find_package(CURL REQUIRED)
find_package(Python3 COMPONENTS Development NumPy)

# Specify the source dirs that are outside the current source tree
set(GTEST_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../third_party/googletest)

# Specify a binary directory where CMake will place the build files for outside dirs
set(GTEST_BINARY_DIR ${CMAKE_BINARY_DIR}/gtest_build)

# Add the GoogleTest directory as a subdirectory, specifying both source and binary directories
add_subdirectory(${GTEST_SOURCE_DIR} ${GTEST_BINARY_DIR})

# Include directories from the root/src directory
include_directories(${CMAKE_SOURCE_DIR}/../src)

# Optionally, if you have header files in a specific include directory
include_directories(${CMAKE_SOURCE_DIR}/../src/include)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})

set(PROJECT_NAME mytests)

set(SRC_FILES
    ../src/priceseries.cpp
    ../src/print_utils.cpp
    ../src/time_utils.cpp
    ../src/overlays/sma.cpp
    ../src/overlays/ema.cpp
    ../src/overlays/bollinger.cpp
    ../src/overlays/rsi.cpp
    ../src/overlays/macd.cpp
)

set(TEST_SRC_FILES
    priceseries_test.cpp
    sma_test.cpp
    ema_test.cpp
)

add_executable(${PROJECT_NAME} 
    ${SRC_FILES}
    ${TEST_SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} 
    GTest::gtest_main
    fmt::fmt
    CURL::libcurl
    ${Python3_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME} DISCOVERY_MODE PRE_TEST)