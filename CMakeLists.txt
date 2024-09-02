cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(cpp_finance VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the fmt and curl packages
find_package(fmt REQUIRED)
find_package(CURL REQUIRED)
find_package(Python3 COMPONENTS Development NumPy)

# Include directories
include_directories(src/include)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})

# Add third parties as a subdirectory
# add_subdirectory(third_party/eigen-3.4.0)

# Add the source directory and files
set(SRC_FILES
    src/example.cpp
    src/priceseries.cpp
    src/print_utils.cpp
    src/time_utils.cpp
    src/overlays/bollinger.cpp
    src/overlays/ema.cpp
    src/overlays/sma.cpp
    src/overlays/macd.cpp
    src/overlays/rsi.cpp
    src/timeseries/ar.cpp
)

# Add an executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt CURL::libcurl ${Python3_LIBRARIES})

# Optionally add compiler flags (e.g., -Wall, -Wextra, -O2)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)