# Minimum version of CMake.
cmake_minimum_required(VERSION 3.16)

# Set the project
project(ring_buffer_cppcheck)

# Print a status message indicating the project being built
message(STATUS "Building as ${PROJECT_NAME} project (tests/cppcheck)")

# List of source files
set(SRC_FILES
    src/main.c
)

# List of include directories & files
set(INC_DIRS
    inc/
)

# List of required libraries 
set(REQ_LIBS
    ring-buffer
)

# Add component ring-buffer
set(RING_BUFFER_CONF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc/conf CACHE STRING "Directory for configuration files")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../.." ring-buffer)

# Status message
message(STATUS "Building as tests/cppcheck project")

# Create Test_Default
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include the directories for the test executable
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRS})

# Link the required libraries to the test executable
target_link_libraries(${PROJECT_NAME} PRIVATE ${REQ_LIBS})