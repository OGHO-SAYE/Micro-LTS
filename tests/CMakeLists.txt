cmake_minimum_required(VERSION 3.14)

# Fixes the timestamp warning on modern CMake versions
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Gather source dependencies from core app
file(GLOB_RECURSE ENGINE_SRC 
    "../src/trading_engine.cpp"
    "../src/order.cpp"
    "../src/risk_manager.cpp"
)

# Use GLOB to find tests dynamically so CMake won't throw hard errors if they are missing
file(GLOB TEST_SRCS "*.cpp")

if(TEST_SRCS)
    add_executable(run_unit_tests 
        ${TEST_SRCS}
        ${ENGINE_SRC}
    )

    target_include_directories(run_unit_tests PRIVATE 
        ${gtest_SOURCE_DIR}/include 
        ../include
    )

    target_link_libraries(run_unit_tests PRIVATE 
        gtest_main 
        Threads::Threads
    )

    include(GoogleTest)
    gtest_discover_tests(run_unit_tests)
else()
    message(STATUS "No test source files found in tests/ yet. Skipping test executable generation.")
endif()