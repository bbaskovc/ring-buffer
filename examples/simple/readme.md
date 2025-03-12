# SIMPLE Example

The `ring-buffer` component provides an efficient solution for storing and managing data in a circular buffer. This example demonstrates how to initialize the ring buffer, insert and replace data elements, and manage buffer size and free space. It highlights the flexibility of the ring buffer in scenarios where data needs to be processed in a continuous, circular manner, making it suitable for real-time data handling in embedded systems.

## Example Overview

The example provided in this repository demonstrates how to:

- Initialize a ring-buffer instance with custom configurations such as buffer size, element size, and overwrite behavior.
- Insert data into the ring buffer and replace existing elements.
- Retrieve and display the current elements in the buffer.
- Check the number of free elements in the buffer and the size of individual elements.
- Properly clean up the ring buffer instance after use.

## Build Instructions

Follow these steps to build the `ring-buffer` example.

1. Navigate to the project directory:
   ```bash
   cd /workspaces/ring-buffer
   ```

2. Run CMake to configure the build:
   ```bash
   cmake -B build -S examples/simple
   ```

3. Build the project using make:
   ```bash
   make -C build
   ```

## Run the Example

After building the project, you can run the example to see the `ring-buffer` example in action.

1. Run example executable.
   ```bash
   idf.py flash monitor
   ```d/ring_buffer_example_simple
```