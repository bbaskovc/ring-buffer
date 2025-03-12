
# RING-BUFFER

<p align="center">
    <a href="https://github.com/bbaskovc/ring-buffer/blob/main/LICENSE.md"><img src="https://img.shields.io/github/license/bbaskovc/ring-buffer.svg" alt="GitHub License"></a>
    <a href="http://ansicolortags.readthedocs.io/?badge=latest"><img src="https://readthedocs.org/projects/ansicolortags/badge/?version=latest" alt="Documentation Status"></a>
    <a href="https://github.com/bbaskovc/ring-buffer/issues"><img src="https://img.shields.io/github/issues/bbaskovc/ring-buffer.svg" alt="GitHub Issues"></a>
    <a href="https://github.com/bbaskovc/ring-buffer/releases/"><img src="https://img.shields.io/github/release/bbaskovc/ring-buffer.svg" alt="GitHub Release"></a>
    <br/>
    <a href="https://github.com/bbaskovc/ring-buffer/stargazers/"><img src="https://img.shields.io/github/stars/bbaskovc/ring-buffer.svg?style=social&label=Star" alt="GitHub Stars"></a>
    <a href="https://github.com/bbaskovc/ring-buffer/network/"><img src="https://img.shields.io/github/forks/bbaskovc/ring-buffer.svg?style=social&label=Fork" alt="GitHub Forks"></a>
</p>

The Component `ring-buffer` provides an efficient circular buffer for handling data in a fixed-size memory structure. It ensures seamless storage and retrieval with minimal memory overhead, ideal for applications requiring high-performance, low-latency data management.

## Table of Contents

- [Features](#features)
- [Dependencies](#dependencies)
    - [Software](#software)
    - [Components](#components)
- [Folder & File Structure](#folder--file-structure)
- [Configurations](#configurations)
- [Exposed Functions](#exposed-functions)
- [Using the `ring-buffer`](#using-the-ring-buffer)
- [Integrating `ring-buffer` into Your Project](#integrating-ring-buffer-into-your-project)
    - [Prerequisites](#prerequisites)
    - [Adding `ring-buffer` to Your Project](#adding-ring-buffer-to-your-project)
- [Examples](#examples)

## Features

* <b>Fixed-Size Buffer:</b> Efficient use of memory with a circular structure that prevents overflow and underflow.
* <b>FIFO Data Management:</b> First-in, first-out management of data for streamlined operations.
* <b>Multiple Instances:</b> Supports multiple `ring-buffer` instances, each with its own configuration (size and element size).
* <b>Dynamic Element Size:</b> Configurable element size for each instance, allowing for variable data types.
* <b>Low Latency:</b> Fast insertion and retrieval of data with minimal processing overhead.
* <b>Peek and Replace:</b> Retrieve data without removal (peek) and replace data at a specific index in the buffer.
* <b>Efficient Memory Usage:</b> Memory is allocated per instance, making it scalable for different use cases.
* **~~Thread-Safe~~:** Ensures thread-safe operations for multi-threaded environments (if needed).
* **~~Tracing Support~~:** Integrated tracing for debugging.

## Dependencies

### Software

- [CMake](https://cmake.org/)
- [GCC](https://www.mingw-w64.org/)

### Components

No dependencies on other components except standard ones.

## Folder & File Structure

```bash
ring-buffer/
├── .devcontainer/              # Configuration for development container (e.g., VS Code dev container).
├── .github/                    # GitHub configuration directory (e.g., Actions, workflows).
├── examples/                   # Sample applications demonstrating how to use the `ring-buffer` component.
├── inc/                        # Public headers for the `ring-buffer` interface.
├── src/                        # Source files implementing `ring-buffer` functionality.
├── tests/                      # Unit tests and validation for the component.
├── .clang-format               # Configuration file for code formatting with Clang.
├── .gitignore                  # Specifies files and directories to be ignored by Git.
├── .gitmodules                 # Git submodule configuration.
├── ring-buffer.code-workspace     # Configuration for VS Code workspace.
├── CHANGELOG.md                # Changelog tracking.
├── CMakeLists.txt              # CMake configuration for building the component.
├── LICENSE.md                  # License terms for the component.
└── README.md                   # Documentation and usage instructions.
```

## Configurations 

All available customization to component functionality can be found below. Click [here](/examples/simple/inc/conf/ring_buffer_conf.h) to view configurations example.

```bash
RING_BUFFER_CONF_FREERTOS_USE   false               # Set to true to enable thread-safety mechanisms.
RING_BUFFER_CONF_TRACE_USE      false               # Set to true to enable logging of buffer actions using TRACE. 
RING_BUFFER_CONF_TRACE_LEVEL    TRACE_LEVEL_VER     # Configure trace level (if tracing is used).
```

## Exposed Functions

```c
// Initialize the ring-buffer instance with the given configuration.
ring_buffer_status_e RING_BUFFER_Init(ring_buffer_t *rb, ring_buffer_conf_t conf);

// De-Initialize the ring-buffer and free allocated memory.
ring_buffer_status_e RING_BUFFER_DeInit(ring_buffer_t *rb);

// Insert data into the ring-buffer.
ring_buffer_status_e RING_BUFFER_Insert(ring_buffer_t *rb, const void *data);

// Retrieve data from the ring-buffer.
ring_buffer_status_e RING_BUFFER_Retrieve(ring_buffer_t *rb, void *data);

// Peek at the data at a specific index without removing it from the buffer.
ring_buffer_status_e RING_BUFFER_Peek(ring_buffer_t *rb, size_t index, void *data);

// Replace data at a specific index with new data.
ring_buffer_status_e RING_BUFFER_Replace(ring_buffer_t *rb, size_t index, const void *data);

// Check if the ring-buffer is empty.
ring_buffer_status_e RING_BUFFER_IsEmpty(ring_buffer_t *rb);

// Check if the ring-buffer is full.
ring_buffer_status_e RING_BUFFER_IsFull(ring_buffer_t *rb);

// Get the number of free elements in the buffer.
ring_buffer_status_e RING_BUFFER_GetFreeElements(ring_buffer_t *rb, size_t *result);

// Get the size of one element in the buffer.
ring_buffer_status_e RING_BUFFER_GetElementSize(ring_buffer_t *rb, size_t *result);
```

## Using the `ring-buffer`

This example demonstrates how to initialize and use a `ring-buffer` with multiple instances, dynamic element sizes, and custom buffer space.

```c
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ring_buffer/ring_buffer.h"

// Define a buffer for the ring-buffer instance
char buffer[256 * sizeof(int)];

// Create a ring-buffer handle
ring_buffer_t rb;

void printBuffer(void)
{
    for (size_t i = 0; i < sizeof(buffer); i++)
    {
        int data;
        if (RING_BUFFER_STATUS_OK == RING_BUFFER_Peek(&rb, i, &data))
        {
            printf("Element at index %zu: %d\n", i, data);
        }
    }
}

int main(void)
{
    ring_buffer_conf_t conf = {
        .buffer = (uint8_t*)buffer,
        .buffer_size = sizeof(buffer),
        .element_size = sizeof(int),
        .overwrite = true,
    };

    // Initialize the ring-buffer with a custom buffer space, element size, and size
    RING_BUFFER_Init(&rb, conf);

    // Insert some data into the ring-buffer
    int data1 = 10;
    RING_BUFFER_Insert(&rb, &data1);

    int data2 = 20;
    RING_BUFFER_Insert(&rb, &data2);

    int data3 = 30;
    RING_BUFFER_Insert(&rb, &data3);

    // Print the current elements in the ring-buffer
    printf("Current elements in the buffer:\n");
    printBuffer();

    // Replace the element at index 1 with a new value
    int newData = 99;
    RING_BUFFER_Replace(&rb, 1, &newData);

    // Print the buffer again after replacement
    printf("\nAfter replacing the element at index 1:\n");
    printBuffer();

    // Get and print the number of free elements
    size_t freeElements;
    RING_BUFFER_GetFreeElements(&rb, &freeElements);
    printf("\nNumber of free elements: %zu\n", freeElements);

    // Get and print the size of one element
    size_t elementSize;
    RING_BUFFER_GetElementSize(&rb, &elementSize);
    printf("Size of one element: %zu bytes\n", elementSize);

    // Clean up (optional, if necessary)
    RING_BUFFER_DeInit(&rb);

    return 0;
}
```

## Integrating `ring-buffer` into Your project  

### Prerequisites  

Ensure you have CMake installed and a compatible toolchain for your target platform.  

### Adding `ring-buffer` to Your Project  

To include `ring-buffer` in your project, follow these steps:  

1. **Clone the repository** (or add it as a submodule):  
    Clone repository:
    ```bash
    git clone https://github.com/bbaskovc/ring-buffer components/ring-buffer
    ```

    Add submodule:
    ```bash
    git submodule add https://github.com/your-repo/ring-buffer.git components/ring-buffer
    ```

2. **Modify** your project's CMakeLists.txt to include the component:
    ```cmake
    # Minimum required version of CMake
    cmake_minimum_required(VERSION 3.16)

    # Set the project name and version
    project(my_project)

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
    set(CALC_CRC_CONF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc/conf CACHE STRING "Directory for configuration files")

    # Add the ring-buffer component
    add_subdirectory(components/ring-buffer)

    # Create the executable 
    add_executable(${PROJECT_NAME} ${SRC_FILES})  

    # Include the directories
    target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRS})

    # Link the required libraries
    target_link_libraries(${PROJECT_NAME} ${REQ_LIBS})
    ```

3. **Build the project:**
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```

## Examples

Here are some examples of different types of entries you might check before use.

- [simple](examples/simple/) Example to show how to use RING BUFFER functionality.
