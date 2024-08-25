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

# Add GoogleTest
add_subdirectory(${CMAKE_SOURCE_DIR}/../third_party/googletest ${CMAKE_BINARY_DIR}/gtest_build)

# Include directories from the root/src directory
include_directories(${CMAKE_SOURCE_DIR}/../src)
include_directories(${CMAKE_SOURCE_DIR}/../src/include)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})

# Source files for the project
set(SRC_FILES
    ${CMAKE_SOURCE_DIR}/../src/priceseries.cpp
    ${CMAKE_SOURCE_DIR}/../src/print_utils.cpp
    ${CMAKE_SOURCE_DIR}/../src/time_utils.cpp
    ${CMAKE_SOURCE_DIR}/../src/overlays/sma.cpp
    ${CMAKE_SOURCE_DIR}/../src/overlays/ema.cpp
    ${CMAKE_SOURCE_DIR}/../src/overlays/bollinger.cpp
    ${CMAKE_SOURCE_DIR}/../src/overlays/rsi.cpp
    ${CMAKE_SOURCE_DIR}/../src/overlays/macd.cpp
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
