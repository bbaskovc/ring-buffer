# CTest Readme

Welcome to the CTest project! This document provides an overview of the project structure and instructions for building and running the tests.

## Project Structure

The project is organized as follows:

```bash
tests/ctest/        
├── CMakeLists.txt              # The CMake configuration file for building the project.
├── inc/                        # Directory containing header files.
│   ├── conf/                   # Configuration header files.
│   │   └── ring_buffer_conf.h  # Configuration for the ring buffer.
│   └── ctest.h                 # Main header file for CTest.
└── src/                        # Directory containing source files.
    └── tests/                  # Directory containing test source files.
        └── main.c              # Main test file.
```

## Building the Project

To build the project, follow these steps:

1. Navigate to the project directory:
   ```bash
   cd /workspaces/ring-buffer
   ```

2. Create a build directory and navigate into it:
   ```bash
   mkdir build
   cd build
   ```

3. Run CMake to configure the build:
   ```bash
   cmake ../tests/ctest
   ```

4. Build the project using make:
   ```bash
   make
   ```

## Running the Tests

After building the project, you can run the tests using the following command:
```bash
./ring_buffer_ctest_test_main
```