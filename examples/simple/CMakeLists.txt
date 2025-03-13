# Minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Set the project name and version
project(calc_crc_example_simple)

# Print a status message indicating the project being built
message(STATUS "Building as ${PROJECT_NAME} project (examples/simple)")

# Add project source files
set(SRC_FILES
    src/main.c
)

# Add project include directories
set(INC_DIRS
    inc/ 
)

# Add project libraries for linking
set(REQ_LIBS
    ring-buffer
)

# Configure the ring-buffer component
set(RING_BUFFER_CONF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc/conf CACHE STRING "Directory for configuration files")

# Add the ring-buffer component
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../ ring-buffer)

# Create the executable 
add_executable(${PROJECT_NAME} ${SRC_FILES})  

# Include the directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRS})

# Link the required libraries
target_link_libraries(${PROJECT_NAME} ${REQ_LIBS})