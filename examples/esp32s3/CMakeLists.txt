# Minimum version of CMake.
cmake_minimum_required(VERSION 3.16)

# Include ESP IDF CMake.
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set the project name.
project(ring_buffer_example_esp32s3)