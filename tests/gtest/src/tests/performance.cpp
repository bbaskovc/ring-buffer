/***********************************************************************************************************************
 *
 * @file        performance.cpp
 * @brief       Test to for testing component performance with googletest.
 * @author      Blaz Baskovc
 * @copyright   Copyright 2025 Blaz Baskovc
 * @date        2025-03-11
 *
 **********************************************************************************************************************/

// --- Includes --------------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include <gtest/gtest.h>

#include "ring_buffer/ring_buffer.h"

// --- Performance Tests -----------------------------------------------------------------------------------------------

TEST(PerformanceTest, InsertPerformance_1k)
{
    char buffer[1024];

    ring_buffer_t rb;
    ring_buffer_conf_t conf = {
        .buffer = (uint8_t *)buffer,
        .buffer_size = sizeof(buffer),
        .element_size = sizeof(uint8_t),
        .overwrite = true,
    };

    RING_BUFFER_Init(&rb, conf);

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < sizeof(buffer); i++)
    {
        RING_BUFFER_Insert(&rb, &buffer[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    RING_BUFFER_DeInit(&rb);

    ASSERT_LT(elapsed.count(), 1.0);
}

TEST(PerformanceTest, InsertPerformance_1M)
{
    char buffer[1024 * 1024];

    ring_buffer_t rb;
    ring_buffer_conf_t conf = {
        .buffer = (uint8_t *)buffer,
        .buffer_size = sizeof(buffer),
        .element_size = sizeof(uint8_t),
        .overwrite = true,
    };

    RING_BUFFER_Init(&rb, conf);

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < sizeof(buffer); i++)
    {
        RING_BUFFER_Insert(&rb, &buffer[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    RING_BUFFER_DeInit(&rb);

    ASSERT_LT(elapsed.count(), 1.0);
}

// --- EOF -------------------------------------------------------------------------------------------------------------
