# Minimum required version of CMake
cmake_minimum_required(VERSION 3.16)

# Set the project name and version
project(ring_buffer_ctest)

# Print a status message indicating the project being built
message(STATUS "Building as ${PROJECT_NAME} project (tests/ctest)")

# Define a list of include directories
set(INC_DIRS
    inc/ 
)

# Define a list of required libraries for linking
set(REQ_LIBS
    ctest
    ring-buffer
)

# Set the C and C++ compiler standards
set(CMAKE_C_STANDARD 99)   
set(CMAKE_CXX_STANDARD 17) 

# Ensure verbose output when running tests with CTest
set(CTEST_OUTPUT_ON_FAILURE TRUE)  
set(CTEST_VERBOSE TRUE) 

# Set additional compiler flags for warnings, security, and coverage
set(RING_BUFFER_ADDITIONAL_FLAGS 
    -Wall                       # Enable all warning messages
    -Wunused-parameter          # Warn about unused function parameters
    -Wshadow                    # Warn about variable shadowing
    -Wconversion                # Warn about implicit type conversions
    -Wmissing-include-dirs      # Warn about missing include directories
    -Wfatal-errors              # Treat warnings as errors
    -Wformat-security           # Warn about format string vulnerabilities
    -Wundef                     # Warn about undefined preprocessor macros
    -Wpointer-arith             # Warn about pointer arithmetic
    --coverage                  # Enable coverage reporting
)

# Set additional compiler flags for warnings, security, and coverage
set(ADDITIONAL_FLAGS 
    --coverage                  # Enable coverage reporting
)

# Enable testing functionality in CMake
enable_testing()

# Add the additional linker flags for coverage
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# Add component ctest
include(FetchContent)
FetchContent_Declare(
  ctest
  GIT_REPOSITORY https://github.com/bbaskovc/ctest.git
  GIT_TAG release/v1.0.0
)
FetchContent_MakeAvailable(ctest)

# Configure the ring-buffer component (ring buffer configuration)
set(RING_BUFFER_TESTING ON)
set(RING_BUFFER_BUILD_FLAGS ${ADDITIONAL_FLAGS})
set(RING_BUFFER_CONF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc/conf CACHE STRING "Directory for configuration files")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ring-buffer)

# Create the executable for the main test, 'Main'
set(TEST_MAIN ${PROJECT_NAME}_test_main)
set(TEST_MAIN_NAME Main)  
add_executable(${TEST_MAIN} src/tests/main.c)  

# Include the directories for the test executable
target_include_directories(${TEST_MAIN} PRIVATE ${INC_DIRS})

# Link the required libraries to the test executable
target_link_libraries(${TEST_MAIN} ${REQ_LIBS})

# Add the additional compiler flags to the test executable
target_compile_options(${TEST_MAIN} PRIVATE ${ADDITIONAL_FLAGS})

# Register the test executable with CTest
add_test(NAME ${TEST_MAIN_NAME} COMMAND ${TEST_MAIN})
