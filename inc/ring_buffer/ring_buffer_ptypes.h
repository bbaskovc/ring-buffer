/***********************************************************************************************************************
 *
 * @file        ring_buffer_ptypes.h
 * @brief       The component RING-BUFFER private types (do not use it outside of component).
 * @author      Blaz Baskovc
 * @copyright   Copyright 2025 Blaz Baskovc
 * @date        2025-02-17
 *
 **********************************************************************************************************************/

#ifndef RING_BUFFER_PTYPES_H
#define RING_BUFFER_PTYPES_H

// C++ wrapper - Start
#ifdef __cplusplus
extern "C" {
#endif

// --- Includes --------------------------------------------------------------------------------------------------------

#include <stdbool.h>
#include <stddef.h>

// --- Private Macros --------------------------------------------------------------------------------------------------

/**
 * @brief   Macro to check for a NULL pointer and execute an action if the pointer is NULL.
 * @param   ptr Pointer to be checked for NULL.
 * @param   action Action to be executed if the pointer is NULL.
 */
#define CHECK_ARGS_NULL_PTR(ptr, action)                                                                               \
    if (NULL == ptr)                                                                                                   \
    {                                                                                                                  \
        return action;                                                                                                 \
    }

/**
 * @brief   Macro to check for a NULL pointer and execute an action if the pointer is NULL.
 * @param   value Value of size_t to be checked for 0.
 * @param   action Action to be executed if the size is 0.
 */
#define CHECK_ARGS_SIZE(value, check, action)                                                                          \
    if (check == value)                                                                                                \
    {                                                                                                                  \
        return action;                                                                                                 \
    }

/**
 * @brief   Set destination memory buffer to byte value.
 * @param   dst Destination buffer.
 * @param   val Overwrite value.
 * @param   len Buffer length.
 */
#define MEMSET(dst, val, len)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        uint8_t *_dstp = (uint8_t *)(dst);                                                                             \
        if (_dstp != NULL)                                                                                             \
        {                                                                                                              \
            for (size_t _i = 0; _i < (len); _i++)                                                                      \
            {                                                                                                          \
                _dstp[_i] = (uint8_t)(val);                                                                            \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

/**
 * @brief   Macro to copy a block of memory from a source to a destination.
 * @param   dst Destination memory block where the data will be copied.
 * @param   src Source memory block to copy the data from.
 * @param   len Number of bytes to copy.
 */
#define MEMCPY(dst, src, len)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        const uint8_t *_srcp = (uint8_t *)(src);                                                                       \
        uint8_t *_dstp = (uint8_t *)(dst);                                                                             \
        if ((_srcp != NULL) && (_dstp != NULL))                                                                        \
        {                                                                                                              \
            for (size_t _i = 0; _i < (len); _i++)                                                                      \
            {                                                                                                          \
                _dstp[_i] = _srcp[_i];                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

// --- Private Types Prototypes ----------------------------------------------------------------------------------------

// C++ wrapper - End
#ifdef __cplusplus
}
#endif

#endif /* RING_BUFFER_PTYPES_H */

// --- EOF -------------------------------------------------------------------------------------------------------------
