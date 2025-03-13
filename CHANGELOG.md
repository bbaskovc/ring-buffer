# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

## [vX.X.X](https://github.com/bbaskovc/ring-buffer/tree/vX.X.X) (2025-XX-XX)

### 🚀 Features

- Moved ctest out of includes and used git repository.

## [v1.0.0](https://github.com/bbaskovc/ring-buffer/tree/v1.0.0) (2025-03-12)

### 🚀 Features

- **RING_BUFFER_Init**: Initializes the ring buffer instance with the provided configuration (buffer, buffer_size, element_size, overwrite flag).
- **RING_BUFFER_DeInit**: De-initializes the ring buffer and frees any dynamically allocated memory.
- **RING_BUFFER_Insert**: Allows inserting data into the ring buffer, handling overwrite behavior based on configuration.
- **RING_BUFFER_Retrieve**: Retrieves data from the ring buffer, removing the item from the buffer after fetching.
- **RING_BUFFER_Peek**: Peeks at the data at a specific index without removing it from the buffer.
- **RING_BUFFER_Replace**: Replaces the data at a given index with new data, enabling modification of specific elements.
- **RING_BUFFER_IsEmpty**: Checks if the ring buffer is empty, providing a simple way to verify buffer state.
- **RING_BUFFER_IsFull**: Checks if the ring buffer is full, indicating whether more data can be inserted.
- **RING_BUFFER_GetFreeElements**: Retrieves the number of free (unused) elements in the ring buffer, useful for buffer management.
- **RING_BUFFER_GetElementSize**: Gets the size of a single element in the buffer, providing flexibility for different data types.

### 📚 Documentation

- **API Docs**: Updated the API documentation to include test functions and how they integrate with the `ring-buffer` component.
- **Development Setup**: Added documentation on setting up the development environment with DevContainer and running tests with CTest and GTest.

### 🧪 Tests

- Added unit tests for the `ring-buffer` component using Googletest (gtest).
- Added unit tests for the `ring-buffer` component using CTest (ctest).
- Added test for the `ring-buffer` component using CppCheck (cppcheck).

### ⚙️ CI

- Added a GitHub Actions workflow to automate the build, test, and static analysis process using CppCheck.

### 📊 Examples

- Added example code `simple` to show how to use component `ring-buffer`.
- Added example code `esp32s3` to show how to use component `ring-buffer` in ESP-IDF environment.