/***********************************************************************************************************************
 *
 * @file        ring_buffer.h
 * @brief       The Component RING BUFFER provides an efficient circular buffer for handling data in a fixed-size memory
 *              structure. It ensures seamless storage and retrieval with minimal memory overhead, ideal for
 *              applications requiring high-performance, low-latency data management.
 *              - Using dynamic memory
 * @author      Blaz Baskovc
 * @copyright   Copyright 2025 Blaz Baskovc
 * @date        2025-03-12
 * @version     1.0.0
 *
 *  Author          Date        Version     Comments on that revision
 * ---------------------------------------------------------------------------------------------------------------------
 *  Blaz Baskovc    2025-03-12  1.0.0       First release.
 *
 **********************************************************************************************************************/

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

// C++ wrapper - Start
#ifdef __cplusplus
extern "C" {
#endif

// --- Includes --------------------------------------------------------------------------------------------------------

#include <stdbool.h>
#include <stddef.h>

#if __has_include("ring_buffer_conf.h")
#include "ring_buffer_conf.h"
#endif /* __has_include("ring_buffer_conf.h") */

#include "ring_buffer/ring_buffer_gtypes.h"

// --- Public Defines --------------------------------------------------------------------------------------------------

/**
 * @brief   Component version.
 */
#define RING_BUFFER_NAME    "RING-BUFFER" //< Component name as string.
#define RING_BUFFER_VERSION (0x01000000U) //< Component version as uint32_t (major.minor.patch.fix).

// --- Public Functions Prototypes -------------------------------------------------------------------------------------

/**
 * @brief Initializes a ring buffer with the given configuration.
 *
 * This function initializes a ring buffer structure by allocating memory and setting the buffer size and other
 * configuration parameters. It must be called before using the ring buffer for reading or writing.
 *
 * @param[in] rb A pointer to the ring buffer structure to be initialized.
 * @param[in] conf The configuration structure containing parameters for the buffer.
 *
 * @return ring_buffer_status_e Status of the initialization:
 *         - RING_BUFFER_STATUS_OK: Successful initialization
 */
ring_buffer_status_e RING_BUFFER_Init(ring_buffer_t *rb, ring_buffer_conf_t conf);

/**
 * @brief Deinitialize a ring buffer and frees any allocated resources.
 *
 * This function deinitialize the ring buffer, releasing any resources associated with it and resetting its internal
 * state. After calling this function, the ring buffer is no longer usable unless reinitialized.
 *
 * @param[in] rb A pointer to the ring buffer structure to be deinitialize.
 *
 * @return ring_buffer_status_e Status of the deinitialization:
 *         - RING_BUFFER_STATUS_OK: Successful deinitialization
 */
ring_buffer_status_e RING_BUFFER_DeInit(ring_buffer_t *rb);

/**
 * @brief Inserts data into the ring buffer.
 *
 * This function inserts a data element into the ring buffer. If there is enough space in the buffer, the data is added;
 * otherwise, it may return an error indicating that the buffer is full.
 *
 * @param[in] rb A pointer to the ring buffer structure where the data will be inserted.
 * @param[in] data A pointer to the data to be inserted into the buffer.
 *
 * @return ring_buffer_status_e Status of the insertion:
 *         - RING_BUFFER_STATUS_OK: Data successfully inserted into the buffer
 */
ring_buffer_status_e RING_BUFFER_Insert(ring_buffer_t *rb, const void *data);

/**
 * @brief Retrieves data from the ring buffer.
 *
 * This function retrieves a data element from the ring buffer. If data is available, it is copied into the provided
 * buffer. If the buffer is empty, it may return an error indicating that no data is available for retrieval.
 *
 * @param[in] rb A pointer to the ring buffer structure from which data will be retrieved.
 * @param[out] data A pointer to the buffer where the retrieved data will be stored.
 *
 * @return ring_buffer_status_e Status of the retrieval:
 *         - RING_BUFFER_STATUS_OK: Data successfully retrieved from the buffer
 */
ring_buffer_status_e RING_BUFFER_Retrieve(ring_buffer_t *rb, void *data);

/**
 * @brief Peeks at data from the ring buffer without removing it.
 *
 * This function retrieves a data element from the ring buffer at the specified index without removing it from the
 * buffer. The buffer remains unchanged after the peek operation. If the index is out of range or if the buffer is
 * empty, it may return an error.
 *
 * @param[in] rb A pointer to the ring buffer structure from which data will be peeked.
 * @param[in] index The index of the data element to peek.
 * @param[out] data A pointer to the buffer where the peeked data will be stored.
 *
 * @return ring_buffer_status_e Status of the peek operation:
 *         - RING_BUFFER_STATUS_OK: Data successfully peeked from the buffer
 */
ring_buffer_status_e RING_BUFFER_Peek(ring_buffer_t *rb, size_t index, void *data);

/**
 * @brief Replaces a data element at the specified index in the ring buffer.
 *
 * This function replaces the data element at the given index with new data. The buffer is not altered in size, and the
 * existing data at that index is overwritten. If the index is out of range or if the buffer is empty, an error will be
 * returned.
 *
 * @param[in] rb A pointer to the ring buffer structure where the data will be replaced.
 * @param[in] index The index of the data element to replace.
 * @param[in] data A pointer to the new data that will replace the existing element.
 *
 * @return ring_buffer_status_e Status of the replace operation:
 *         - RING_BUFFER_STATUS_OK: Data successfully replaced at the specified index
 */
ring_buffer_status_e RING_BUFFER_Replace(ring_buffer_t *rb, size_t index, const void *data);

/**
 * @brief Checks if the ring buffer is empty.
 *
 * This function checks whether the ring buffer contains any data. It returns a status  indicating whether the buffer is
 * empty or contains data that can be retrieved.
 *
 * @param[in] rb A pointer to the ring buffer structure to check.
 *
 * @return ring_buffer_status_e Status of the check:
 *         - RING_BUFFER_STATUS_OK: The buffer is empty
 */
ring_buffer_status_e RING_BUFFER_IsEmpty(ring_buffer_t *rb);

/**
 * @brief Checks if the ring buffer is full.
 *
 * This function checks whether the ring buffer is full and cannot accommodate more data.
 * It returns a status indicating whether the buffer is full or still has space available.
 *
 * @param[in] rb A pointer to the ring buffer structure to check.
 *
 * @return ring_buffer_status_e Status of the check:
 *         - RING_BUFFER_STATUS_OK: The buffer is full and cannot accept more data
 */
ring_buffer_status_e RING_BUFFER_IsFull(ring_buffer_t *rb);

/**
 * @brief Gets the number of free elements in the ring buffer.
 *
 * This function retrieves the number of available (free) slots in the ring buffer
 * that can be used to insert new data. It returns the count of unused elements.
 *
 * @param[in] rb A pointer to the ring buffer structure to check.
 * @param[out] result A pointer to a variable where the number of free elements will be stored.
 *
 * @return ring_buffer_status_e Status of the operation:
 *         - RING_BUFFER_STATUS_OK: The number of free elements was successfully retrieved.
 */
ring_buffer_status_e RING_BUFFER_GetFreeElements(ring_buffer_t *rb, size_t *result);

/**
 * @brief Gets the size of each element in the ring buffer.
 *
 * This function retrieves the size of each element stored in the ring buffer.
 * It is useful when the buffer stores elements of varying sizes or when a
 * consistent element size is required for processing.
 *
 * @param[in] rb A pointer to the ring buffer structure to check.
 * @param[out] result A pointer to a variable where the size of element will be stored.
 *
 * @return ring_buffer_status_e Status of the operation:
 *         - RING_BUFFER_STATUS_OK: The element size was successfully retrieved.
 */
ring_buffer_status_e RING_BUFFER_GetElementSize(ring_buffer_t *rb, size_t *result);

// C++ wrapper - End
#ifdef __cplusplus
}
#endif

#endif /* RING_BUFFER_H */

// --- EOF -------------------------------------------------------------------------------------------------------------
