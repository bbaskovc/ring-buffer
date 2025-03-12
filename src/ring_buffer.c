/***********************************************************************************************************************
 *
 * @file        ring_buffer.h
 * @brief       The Component RING BUFFER provides an efficient circular buffer for handling data in a fixed-size memory
 *              structure. It ensures seamless storage and retrieval with minimal memory overhead, ideal for
 *              applications requiring high-performance, low-latency data management.
 *              - Using dynamic memory
 * @author      Blaz Baskovc
 * @copyright   Copyright 2025 Blaz Baskovc
 * @date        2025-02-24
 * @version     1.0.0
 *
 *  Author          Date        Version     Comments on that revision
 * ---------------------------------------------------------------------------------------------------------------------
 *  Blaz Baskovc    2025-03-12  1.0.0       First release.
 *
 **********************************************************************************************************************/

// --- Includes --------------------------------------------------------------------------------------------------------

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#if __has_include("ring_buffer_conf.h")
#include "ring_buffer_conf.h"
#endif /* __has_include("ring_buffer_conf.h") */

#ifdef RING_BUFFER_CONF_FREERTOS_USE
#if (true == RING_BUFFER_CONF_FREERTOS_USE)

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#endif /* (true == RING_BUFFER_CONF_FREERTOS_USE) */
#endif /* RING_BUFFER_CONF_FREERTOS_USE */

#include "ring_buffer/ring_buffer_ptypes.h"
#include "ring_buffer/ring_buffer_gtypes.h"
#include "ring_buffer/ring_buffer.h"

// --- Public Functions Definitions ------------------------------------------------------------------------------------

ring_buffer_status_e RING_BUFFER_Init(ring_buffer_t *rb, ring_buffer_conf_t conf)
{
    CHECK_ARGS_NULL_PTR(rb, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(conf.buffer, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_SIZE(conf.buffer_size, 0, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_SIZE(conf.element_size, 0, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);

    if (conf.buffer_size < conf.element_size)
    {
        return RING_BUFFER_STATUS_ERROR_INPUT_ARGS;
    }

    MEMSET(rb, 0, sizeof(ring_buffer_t));

    rb->conf = conf;
    rb->max_elements = rb->conf.buffer_size / rb->conf.element_size;

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_status_e RING_BUFFER_DeInit(ring_buffer_t *rb)
{
    CHECK_ARGS_NULL_PTR(rb, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(rb->conf.buffer, RING_BUFFER_STATUS_ERROR_NOT_INITIALIZED);

    MEMSET(rb, 0, sizeof(ring_buffer_t));

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_status_e RING_BUFFER_Insert(ring_buffer_t *rb, const void *data)
{
    CHECK_ARGS_NULL_PTR(rb, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(data, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(rb->conf.buffer, RING_BUFFER_STATUS_ERROR_NOT_INITIALIZED);

    if (false == rb->conf.overwrite && rb->count >= rb->max_elements)
    {
        return RING_BUFFER_STATUS_ERROR_BUFFER_FULL;
    }

    uint8_t *dest = rb->conf.buffer + rb->head;
    size_t end_space = rb->conf.buffer_size - rb->head;
    if (end_space >= rb->conf.element_size)
    {
        MEMCPY(dest, data, rb->conf.element_size);
        rb->head += rb->conf.element_size;
    }
    else
    {
        MEMCPY(dest, data, end_space);
        MEMCPY(rb->conf.buffer, (uint8_t *)data + end_space, rb->conf.element_size - end_space);
        rb->head = rb->conf.element_size - end_space;
    }

    if (rb->head >= rb->conf.buffer_size)
    {
        rb->head = 0;
    }

    if (rb->conf.overwrite)
    {
        if (rb->count >= rb->max_elements)
        {
            rb->tail = (rb->tail + rb->conf.element_size) % rb->conf.buffer_size;
        }
    }

    if (rb->count < rb->max_elements)
    {
        rb->count++;
    }

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_status_e RING_BUFFER_Retrieve(ring_buffer_t *rb, void *data)
{
    CHECK_ARGS_NULL_PTR(rb, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(data, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(rb->conf.buffer, RING_BUFFER_STATUS_ERROR_NOT_INITIALIZED);
    CHECK_ARGS_SIZE(rb->count, 0, RING_BUFFER_STATUS_ERROR_BUFFER_EMPTY);

    uint8_t *src = rb->conf.buffer + rb->tail;
    size_t end_space = rb->conf.buffer_size - rb->tail;

    if (end_space >= rb->conf.element_size)
    {
        MEMCPY(data, src, rb->conf.element_size);
        rb->tail += rb->conf.element_size;
    }
    else
    {
        MEMCPY(data, src, end_space);
        MEMCPY((uint8_t *)data + end_space, rb->conf.buffer, rb->conf.element_size - end_space);
        rb->tail = rb->conf.element_size - end_space;
    }

    rb->count--;

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_status_e RING_BUFFER_Peek(ring_buffer_t *rb, size_t index, void *data)
{
    CHECK_ARGS_NULL_PTR(rb, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(data, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(rb->conf.buffer, RING_BUFFER_STATUS_ERROR_NOT_INITIALIZED);
    CHECK_ARGS_SIZE(rb->count, 0, RING_BUFFER_STATUS_ERROR_BUFFER_EMPTY);

    if (index >= rb->count)
    {
        return RING_BUFFER_STATUS_ERROR_INVALID_INDEX;
    }

    size_t element_pos = (rb->tail + index * rb->conf.element_size) % rb->conf.buffer_size;
    uint8_t *src = rb->conf.buffer + element_pos;
    size_t end_space = rb->conf.buffer_size - element_pos;

    if (end_space >= rb->conf.element_size)
    {
        MEMCPY(data, src, rb->conf.element_size);
    }
    else
    {
        MEMCPY(data, src, end_space);
        MEMCPY((uint8_t *)data + end_space, rb->conf.buffer, rb->conf.element_size - end_space);
    }

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_status_e RING_BUFFER_Replace(ring_buffer_t *rb, size_t index, const void *data)
{
    CHECK_ARGS_NULL_PTR(rb, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(data, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(rb->conf.buffer, RING_BUFFER_STATUS_ERROR_NOT_INITIALIZED);
    CHECK_ARGS_SIZE(rb->count, 0, RING_BUFFER_STATUS_ERROR_BUFFER_EMPTY);

    if (index >= rb->count)
    {
        return RING_BUFFER_STATUS_ERROR_INVALID_INDEX;
    }

    size_t element_pos = (rb->tail + index * rb->conf.element_size) % rb->conf.buffer_size;
    uint8_t *dest = rb->conf.buffer + element_pos;
    size_t end_space = rb->conf.buffer_size - element_pos;

    if (end_space >= rb->conf.element_size)
    {
        MEMCPY(dest, data, rb->conf.element_size);
    }
    else
    {
        MEMCPY(dest, data, end_space);
        MEMCPY(rb->conf.buffer, (uint8_t *)data + end_space, rb->conf.element_size - end_space);
    }

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_status_e RING_BUFFER_IsEmpty(ring_buffer_t *rb)
{
    CHECK_ARGS_NULL_PTR(rb, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(rb->conf.buffer, RING_BUFFER_STATUS_ERROR_NOT_INITIALIZED);

    if (0 == rb->count)
    {
        return RING_BUFFER_STATUS_OK;
    }

    return RING_BUFFER_STATUS_ERROR;
}

ring_buffer_status_e RING_BUFFER_IsFull(ring_buffer_t *rb)
{
    CHECK_ARGS_NULL_PTR(rb, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(rb->conf.buffer, RING_BUFFER_STATUS_ERROR_NOT_INITIALIZED);

    if (rb->max_elements == rb->count)
    {
        return RING_BUFFER_STATUS_OK;
    }

    return RING_BUFFER_STATUS_ERROR;
}

ring_buffer_status_e RING_BUFFER_GetFreeElements(ring_buffer_t *rb, size_t *result)
{
    CHECK_ARGS_NULL_PTR(rb, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(result, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(rb->conf.buffer, RING_BUFFER_STATUS_ERROR_NOT_INITIALIZED);

    *result = (size_t)(rb->max_elements - rb->count);

    return RING_BUFFER_STATUS_OK;
}

ring_buffer_status_e RING_BUFFER_GetElementSize(ring_buffer_t *rb, size_t *result)
{
    CHECK_ARGS_NULL_PTR(rb, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(result, RING_BUFFER_STATUS_ERROR_INPUT_ARGS);
    CHECK_ARGS_NULL_PTR(rb->conf.buffer, RING_BUFFER_STATUS_ERROR_NOT_INITIALIZED);

    *result = (size_t)rb->conf.element_size;

    return RING_BUFFER_STATUS_OK;
}

// --- EOF -------------------------------------------------------------------------------------------------------------
