cmake_minimum_required(VERSION 3.14)
project(MicroLTS VERSION 1.0.0 LANGUAGES CXX)

# Enforce a modern C++ standard for high-performance features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific optimization flag tuning for low latency
if(MSVC)
    add_compile_options(/W4 /O2 /permissive-)
else()
    # -O3 for heavy loop/vectorization optimizations
    # -march=native ensures binaries leverage local CPU instruction sets
    add_compile_options(-Wall -Wextra -Wpedantic -O3 -march=native)
endif()

# Define public include paths
include_directories(include)

# Gather implementation source files
file(GLOB_RECURSE SRC_FILES 
    "src/trading_engine.cpp"
    "src/order.cpp"
    "src/risk_manager.cpp"
)

# Core system executable target
add_executable(micro_lts src/main.cpp ${SRC_FILES})

# Link threading library explicitly for concurrent structures
find_package(Threads REQUIRED)
target_link_libraries(micro_lts PRIVATE Threads::Threads)

# Expose testing hooks if enabled
enable_testing()
add_subdirectory(tests)