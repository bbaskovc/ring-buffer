
# Component Ring Buffer

The Component RING BUFFER provides an efficient circular buffer for handling data in a fixed-size memory structure. It ensures seamless storage and retrieval with minimal memory overhead, ideal for applications requiring high-performance, low-latency data management.

## Features

* <b>Fixed-Size Buffer:</b> Efficient use of memory with a circular structure that prevents overflow and underflow.
* <b>FIFO Data Management:</b> First-in, first-out management of data for streamlined operations.
* <b>Multiple Instances:</b> Supports multiple ring buffer instances, each with its own configuration (size and element size).
* <b>Dynamic Element Size:</b> Configurable element size for each instance, allowing for variable data types.
* <b>Thread-Safe:</b> Supports concurrent access for multi-threaded applications (if needed).
* <b>Low Latency:</b> Fast insertion and retrieval of data with minimal processing overhead.
* <b>Peek and Replace:</b> Retrieve data without removal (peek) and replace data at a specific index in the buffer.
* <b>Efficient Memory Usage:</b> Memory is allocated per instance, making it scalable for different use cases.

## Dependencies

### Software

- [CMake](https://cmake.org/)
- [GCC](https://www.mingw-w64.org/)

### Components

No dependencies on other components except standard ones.

## Folder & File Structure

```bash
trace/
├── examples/                                   # Component examples for a quick start.
├── inc/                                        # Include files.
├── src/                                        # Component source files.
├── CHANGELOG.md                                # Changelog tracking.
├── CMakeLists.txt                              # CMake component configuration file.
├── LICENSE.md                                  # License file.
└── README.md                                   # This is the file you are currently reading.
```

## Configurations 

All available customization to component functionality can be found below. Click [here](/examples/default/inc/conf/trace_conf.h) to view configurations example.

```bash
RING_BUFFER_CONF_FREERTOS_USE   true                # Set to true to enable thread-safety mechanisms.
RING_BUFFER_CONF_TRACE_USE      true                # Set to true to enable logging of buffer actions using TRACE. 
RING_BUFFER_CONF_TRACE_LEVEL    TRACE_LEVEL_VER     # Configure trace level (if tracing is used).
```

## Exposed Functions

```c
ring_buffer_status_e RING_BUFFER_Init(ring_buffer_t *rb, ring_buffer_conf_t conf);

// De-Initialize the ring buffer and free allocated memory.
ring_buffer_status_e RING_BUFFER_DeInit(ring_buffer_t *rb);

// Insert data into the ring buffer.
ring_buffer_status_e RING_BUFFER_Insert(ring_buffer_t *rb, const void *data);

// Retrieve data from the ring buffer.
ring_buffer_status_e RING_BUFFER_Retrieve(ring_buffer_t *rb, void *data);

// Peek at the data at a specific index without removing it from the buffer.
ring_buffer_status_e RING_BUFFER_Peek(ring_buffer_t *rb, size_t index, void *data);

// Replace data at a specific index with new data.
ring_buffer_status_e RING_BUFFER_Replace(ring_buffer_t *rb, size_t index, const void *data);

// Check if the ring buffer is empty.
ring_buffer_status_e RING_BUFFER_IsEmpty(ring_buffer_t *rb);

// Check if the ring buffer is full.
ring_buffer_status_e RING_BUFFER_IsFull(ring_buffer_t *rb);

// Get the number of free elements in the buffer.
ring_buffer_status_e RING_BUFFER_GetFreeElements(ring_buffer_t *rb, size_t *result);

// Get the size of one element in the buffer.
ring_buffer_status_e RING_BUFFER_GetElementSize(ring_buffer_t *rb, size_t *result);
```

## Using the Ring Buffer

This example demonstrates how to initialize and use a Ring Buffer with multiple instances, dynamic element sizes, and custom buffer space.

```c
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ring_buffer/ring_buffer.h"

// Define a buffer for the ring buffer instance
char buffer[256 * sizeof(int)];

// Create a ring buffer handle
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

    // Initialize the ring buffer with a custom buffer space, element size, and size
    RING_BUFFER_Init(&rb, conf);

    // Insert some data into the ring buffer
    int data1 = 10;
    RING_BUFFER_Insert(&rb, &data1);

    int data2 = 20;
    RING_BUFFER_Insert(&rb, &data2);

    int data3 = 30;
    RING_BUFFER_Insert(&rb, &data3);

    // Print the current elements in the ring buffer
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

## Examples

Here are some examples of different types of entries you might check before use.

- [default](examples/default/) Example to show how to use RING BUFFER functionality.
