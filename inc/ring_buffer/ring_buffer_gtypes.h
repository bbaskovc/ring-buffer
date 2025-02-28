/***********************************************************************************************************************
 *
 * @file        ring_buffer_gtypes.h
 * @brief       The component RING-BUFFER global types separate to main files to include for defines.
 * @author      Blaz Baskovc
 * @copyright   Copyright 2025 Blaz Baskovc
 * @date        2025-02-24
 *
 **********************************************************************************************************************/

#ifndef RING_BUFFER_GTYPES_H
#define RING_BUFFER_GTYPES_H

// C++ wrapper - Start
#ifdef __cplusplus
extern "C" {
#endif

// --- Includes --------------------------------------------------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// --- Public Types Prototypes -----------------------------------------------------------------------------------------

/**
 * @brief   Enumeration representing the response status of a function.
 */
typedef enum
{
    RING_BUFFER_STATUS_OK = 0u,
    RING_BUFFER_STATUS_ERROR,
    RING_BUFFER_STATUS_ERROR_INPUT_ARGS,
    RING_BUFFER_STATUS_ERROR_NOT_INITIALIZED,
    RING_BUFFER_STATUS_ERROR_INVALID_INDEX,
    RING_BUFFER_STATUS_ERROR_BUFFER_EMPTY,
    RING_BUFFER_STATUS_ERROR_BUFFER_FULL,
    RING_BUFFER_STATUS_ERROR_OVERFLOW,
    RING_BUFFER_STATUS_MAX
} ring_buffer_status_e;

/**
 * @brief   Structure representing a ring buffer configurations.
 */
typedef struct
{
    uint8_t *buffer;     /// Pointer to the ring buffer memory.
    size_t buffer_size;  /// Number of bytes in the buffer.
    size_t element_size; /// Size of one element in bytes.
    bool overwrite;      /// Enable inserting new elements even if full.
} ring_buffer_conf_t;

/**
 * @brief   Structure representing a ring buffer object.
 */
typedef struct
{
    ring_buffer_conf_t conf; /// Ring Buffer configurations.
    size_t head;             /// Index for the next write operation.
    size_t tail;             /// Index for the next read operation.
    size_t count;            /// Number of elements currently in the buffer.
    size_t max_elements;     /// Maximum number of elements that fit in the buffer.
} ring_buffer_t;

// C++ wrapper - End
#ifdef __cplusplus
}
#endif

#endif /* RING_BUFFER_GTYPES_H */

// --- EOF -------------------------------------------------------------------------------------------------------------
