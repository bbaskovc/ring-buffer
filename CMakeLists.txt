# Specify the minimum required version of CMake.
cmake_minimum_required(VERSION 3.16)

# Define the list of source files for the project.
set(SRC_FILES
    src/ring_buffer.c
)

# Define the list of include directories.
set(INC_DIRS
    inc/
)

# Define required libraries (currently empty).
set(REQ_LIBS
)

# The variable ${RING_BUFFER_CONF_DIR} allows for additional configuration includes.
if(RING_BUFFER_CONF_DIR)
    list(APPEND INC_DIRS ${RING_BUFFER_CONF_DIR})
endif()

# Check if the build is targeting ESP-IDF.
if(IDF_TARGET)
    message(STATUS "Building as ESP-IDF project")
    # Register component to ESP-IDF
    idf_component_register(SRCS ${SRC_FILES} INCLUDE_DIRS ${INC_DIRS} REQUIRES ${REQ_LIBS})
else()
    message(STATUS "Building as CMake project")
    # Define the project name.
    project(ring-buffer)

    # Define a static library from the specified source files.
    add_library(${PROJECT_NAME} ${SRC_FILES})
    # Specify the include directories for the target library.
    target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIRS})
    # Link required libraries (empty in this case).
    target_link_libraries(${PROJECT_NAME} PRIVATE ${REQ_LIBS})

    # Apply additional compiler flags for stricter code checks.
    if(RING_BUFFER_BUILD_FLAGS)
        target_compile_options(${PROJECT_NAME} PRIVATE ${RING_BUFFER_BUILD_FLAGS})
    endif()

    # Enable code coverage instrumentation when tests are enabled.
    if(RING_BUFFER_TESTING)
        message(STATUS "Testing is enabled")
        # Add a preprocessor definition for enabling tests in code
        add_definitions(-DRING_BUFFER_TESTING)
        # Apply addition compiler flags for testing purposes.
        target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
         # Enable coverage flags
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    endif()
endif()
