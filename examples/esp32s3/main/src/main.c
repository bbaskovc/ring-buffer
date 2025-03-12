
/***********************************************************************************************************************
 *
 * @file        main.c
 * @brief       Main ESP application task (application start-up).
 * @author      Blaz Baskovc
 * @copyright   Copyright 2025 Blaz Baskovc
 * @date        2025-02-27
 *
 **********************************************************************************************************************/

// --- Includes --------------------------------------------------------------------------------------------------------

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "main.h"
#include "ring_buffer/ring_buffer.h"

// --- Project Main ----------------------------------------------------------------------------------------------------

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

void app_main(void)
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

    // Delay before restart
    vTaskDelay(pdMS_TO_TICKS(60*1000));
}

// --- EOF -------------------------------------------------------------------------------------------------------------
