# Minimum version of CMake.
cmake_minimum_required(VERSION 3.16)

# Set the project
project(ring_buffer_gtest)

# Status message
message(STATUS "Building as ${PROJECT_NAME} project (tests/gtest)")

# Define the list of source files for the project.
set(SRC_FILES
    src/main.cpp
)

# List of includes
set(INC_DIRS
    inc/
)

# List of required libraries 
set(REQ_LIBS
    gtest 
    gtest_main
    ring-buffer
)

# Enable testing functionality in CMake
enable_testing()

# Include Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)


# Configure the ring-buffer component (ring buffer configuration)
set(RING_BUFFER_CONF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc/conf CACHE STRING "Directory for configuration files")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ring-buffer)

# Create the executable for the main test, 'Performance'
set(TEST_PERFORMANCE ${PROJECT_NAME}_test_performance)  
set(TEST_PERFORMANCE_NAME Performance) 
add_executable(${TEST_PERFORMANCE} ${SRC_FILES} src/tests/performance.cpp)

# Include the directories for the test executable
target_include_directories(${TEST_PERFORMANCE} PRIVATE ${INC_DIRS})

# Link the required libraries to the test executable
target_link_libraries(${TEST_PERFORMANCE} PRIVATE ${REQ_LIBS})

# Register the test executable with Google Test
add_test(NAME ${TEST_PERFORMANCE_NAME} COMMAND ${TEST_PERFORMANCE})