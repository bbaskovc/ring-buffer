/***********************************************************************************************************************
 *
 * @file        main.c
 * @brief       Windows / Linux platform start-up file to test DATA-LOGGER component.
 * @author      Blaz Baskovc
 * @copyright   Copyright 2025 Blaž Basškovč
 * @date        2025-02-17
 *
 **********************************************************************************************************************/

// --- Includes --------------------------------------------------------------------------------------------------------

#include "ring_buffer/ring_buffer.h"
#include "ring_buffer/ring_buffer_gtypes.h"
#include "ring_buffer/ring_buffer_ptypes.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

// --- Private Defines -------------------------------------------------------------------------------------------------

/**
 * @brief   Add all tests to run to the list (define it before enable and include of ctest).
 */
#define TESTS                                                                                                          \
    ADD(ring_buffer_init_null_buffer)                                                                                  \
    ADD(ring_buffer_init_null_handle)                                                                                  \
    ADD(ring_buffer_init_zero_size)                                                                                    \
    ADD(ring_buffer_init_zero_element_size)                                                                            \
    ADD(ring_buffer_init_insufficient_size)                                                                            \
    ADD(ring_buffer_init_valid)                                                                                        \
    ADD(ring_buffer_deinit_null_handle)                                                                                \
    ADD(ring_buffer_deinit_valid)                                                                                      \
    ADD(ring_buffer_insert_null_handle)                                                                                \
    ADD(ring_buffer_insert_full_buffer)                                                                                \
    ADD(ring_buffer_insert_overwrite)                                                                                  \
    ADD(ring_buffer_insert_valid)                                                                                      \
    ADD(ring_buffer_retrieve_null_handle)                                                                              \
    ADD(ring_buffer_retrieve_valid)                                                                                    \
    ADD(ring_buffer_peek_null_handle)                                                                                  \
    ADD(ring_buffer_peek_valid)                                                                                        \
    ADD(ring_buffer_replace_null_handle)                                                                               \
    ADD(ring_buffer_replace_empty_buffer)                                                                              \
    ADD(ring_buffer_replace_single_element)                                                                            \
    ADD(ring_buffer_replace_full_buffer)                                                                               \
    ADD(ring_buffer_replace_overwrite_enabled)                                                                         \
    ADD(ring_buffer_replace_valid)                                                                                     \
    ADD(ring_buffer_is_empty_after_insert)                                                                             \
    ADD(ring_buffer_is_empty_after_retrieve)                                                                           \
    ADD(ring_buffer_is_empty_initialized)                                                                              \
    ADD(ring_buffer_is_empty_with_overwrite)                                                                           \
    ADD(ring_buffer_is_empty_no_overwrite)                                                                             \
    ADD(ring_buffer_is_empty_valid)                                                                                    \
    ADD(ring_buffer_is_full_initialized)                                                                               \
    ADD(ring_buffer_is_full_after_insert)                                                                              \
    ADD(ring_buffer_is_full_with_overwrite)                                                                            \
    ADD(ring_buffer_is_full_no_overwrite)                                                                              \
    ADD(ring_buffer_is_full_after_retrieve)                                                                            \
    ADD(ring_buffer_is_full_valid)                                                                                     \
    ADD(ring_buffer_get_free_elements_empty)                                                                           \
    ADD(ring_buffer_get_free_elements_after_insert)                                                                    \
    ADD(ring_buffer_get_free_elements_full)                                                                            \
    ADD(ring_buffer_get_free_elements_with_overwrite)                                                                  \
    ADD(ring_buffer_get_free_elements_after_retrieve)                                                                  \
    ADD(ring_buffer_get_free_elements_valid)                                                                           \
    ADD(ring_buffer_get_element_size_valid)                                                                            \
    ADD(ring_buffer_get_element_size)                                                                                  \
    ADD(ring_buffer_get_element_size_different_size)                                                                   \
    ADD(ring_buffer_get_element_size_initialized)                                                                      \
    ADD(ring_buffer_get_element_size_with_overwrite)                                                                   \
    ADD(ring_buffer_insert_valid)

/**
 * @brief   Enable ctest implementation.
 */
#define CTEST_IMPLEMENTATION

/**
 * @brief   Include ctest implementation
 */
#include "ctest/ctest.h"

// --- Application start-up --------------------------------------------------------------------------------------------

int32_t main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // Disable stdout buffering (needed to print even if one character is in buffer)
    setvbuf(stdout, NULL, _IONBF, 0);

    // Run ctest tests
    return ctest__run_tests() ? EXIT_SUCCESS : EXIT_FAILURE;
}

// --- Private Functions Definitions -----------------------------------------------------------------------------------

static int32_t test_ring_buffer_init_null_buffer(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {.buffer = 0, .buffer_size = 0, .element_size = 0, .overwrite = false};

    t1_result = RING_BUFFER_Init(&t1_rb, t1_conf);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, t1_result,
                  "RING_BUFFER_Init(%p, (%p, %d, %d, %d)) -> Expected %d, but got %d.", &t1_rb, t1_conf.buffer,
                  t1_conf.buffer_size, t1_conf.element_size, t1_conf.overwrite, RING_BUFFER_STATUS_ERROR_INPUT_ARGS,
                  t1_result);

    return failed_assertions;
}

static int32_t test_ring_buffer_init_null_handle(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    uint8_t t1_buffer[rand() % 1024 + 1];
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {
        .buffer = t1_buffer, .buffer_size = sizeof(t1_buffer), .element_size = 1, .overwrite = false};

    t1_result = RING_BUFFER_Init(NULL, t1_conf);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, t1_result,
                  "RING_BUFFER_Init(%p, (%p, %d, %d, %d)) -> Expected %d, but got %d.", NULL, &t1_rb, t1_conf.buffer,
                  t1_conf.buffer_size, t1_conf.element_size, t1_conf.overwrite, RING_BUFFER_STATUS_ERROR_INPUT_ARGS,
                  t1_result);

    return failed_assertions;
}

static int32_t test_ring_buffer_init_zero_size(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    uint8_t t1_buffer[rand() % 1024 + 1];
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {.buffer = t1_buffer, .buffer_size = 0, .element_size = 1, .overwrite = false};

    t1_result = RING_BUFFER_Init(&t1_rb, t1_conf);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, t1_result,
                  "RING_BUFFER_Init(%p, (%p, %d, %d, %d)) -> Expected %d, but got %d.", &t1_rb, &t1_rb, t1_conf.buffer,
                  t1_conf.buffer_size, t1_conf.element_size, t1_conf.overwrite, RING_BUFFER_STATUS_ERROR_INPUT_ARGS,
                  t1_result);

    return failed_assertions;
}

static int32_t test_ring_buffer_init_zero_element_size(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    uint8_t t1_buffer[rand() % 1024 + 1];
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {
        .buffer = t1_buffer, .buffer_size = sizeof(t1_buffer), .element_size = 0, .overwrite = false};

    t1_result = RING_BUFFER_Init(&t1_rb, t1_conf);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, t1_result,
                  "RING_BUFFER_Init(%p, (%p, %d, %d, %d)) -> Expected %d, but got %d.", &t1_rb, &t1_rb, t1_conf.buffer,
                  t1_conf.buffer_size, t1_conf.element_size, t1_conf.overwrite, RING_BUFFER_STATUS_ERROR_INPUT_ARGS,
                  t1_result);

    return failed_assertions;
}

static int32_t test_ring_buffer_init_insufficient_size(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    uint8_t t1_buffer[rand() % 1024 + 1];
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {.buffer = t1_buffer,
                                  .buffer_size = sizeof(t1_buffer),
                                  .element_size = sizeof(t1_buffer) + 1,
                                  .overwrite = false};

    t1_result = RING_BUFFER_Init(&t1_rb, t1_conf);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, t1_result,
                  "RING_BUFFER_Init(%p, (%p, %d, %d, %d)) -> Expected %d, but got %d.", &t1_rb, &t1_rb, t1_conf.buffer,
                  t1_conf.buffer_size, t1_conf.element_size, t1_conf.overwrite, RING_BUFFER_STATUS_ERROR_INPUT_ARGS,
                  t1_result);

    return failed_assertions;
}

static int32_t test_ring_buffer_init_valid(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    uint8_t t1_buffer[rand() % 1024 + 1];
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {
        .buffer = t1_buffer, .buffer_size = sizeof(t1_buffer), .element_size = 1, .overwrite = false};

    MEMSET(&t1_rb, 0xFF, sizeof(ring_buffer_t));

    t1_result = RING_BUFFER_Init(&t1_rb, t1_conf);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, t1_result,
                  "RING_BUFFER_Init(%p, (%p, %d, %d, %d)) -> Expected %d, but got %d.", &t1_rb, &t1_rb, t1_conf.buffer,
                  t1_conf.buffer_size, t1_conf.element_size, t1_conf.overwrite, RING_BUFFER_STATUS_OK, t1_result);
    CTEST_ASSERT_EQ_MSG(sizeof(t1_buffer), t1_rb.conf.buffer_size, "Expected %d, but got %d.", sizeof(t1_buffer),
                  t1_rb.conf.buffer_size);
    CTEST_ASSERT_EQ_MSG(1, t1_rb.conf.element_size, "Expected %d, but got %d.", 1, t1_rb.conf.element_size);
    CTEST_ASSERT_EQ_MSG(t1_buffer, t1_rb.conf.buffer, "Expected %d, but got %d.", t1_buffer, t1_rb.conf.buffer);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.count, "Expected %d, but got %d.", 0, t1_rb.count);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.head, "Expected %d, but got %d.", 0, t1_rb.head);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.tail, "Expected %d, but got %d.", 0, t1_rb.tail);
    CTEST_ASSERT_EQ_MSG(sizeof(t1_buffer), t1_rb.max_elements, "Expected %d, but got %d.", sizeof(t1_buffer),
                  t1_rb.max_elements);

    return failed_assertions;
}

static int32_t test_ring_buffer_deinit_null_handle(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;

    // Deinitialize with a null pointer
    result = RING_BUFFER_DeInit(NULL);

    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result, "RING_BUFFER_DeInit(NULL) -> Expected %d, but got %d.",
                  RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_deinit_valid(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    uint8_t t1_buffer[rand() % 1024 + 1];
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {
        .buffer = t1_buffer, .buffer_size = sizeof(t1_buffer), .element_size = 1, .overwrite = false};

    MEMSET(&t1_rb, 0xFF, sizeof(ring_buffer_t));

    t1_result = RING_BUFFER_Init(&t1_rb, t1_conf);
    t1_result = RING_BUFFER_DeInit(&t1_rb);

    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, t1_result, "RING_BUFFER_DeInit(%p) -> Expected %d, but got %d.", &t1_rb,
                  RING_BUFFER_STATUS_OK, t1_result);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.conf.buffer_size, "Expected %d, but got %d.", 0, t1_rb.conf.buffer_size);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.conf.element_size, "Expected %d, but got %d.", 0, t1_rb.conf.element_size);
    CTEST_ASSERT_EQ_MSG(NULL, t1_rb.conf.buffer, "Expected %d, but got %d.", NULL, t1_rb.conf.buffer);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.count, "Expected %d, but got %d.", 0, t1_rb.count);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.head, "Expected %d, but got %d.", 0, t1_rb.head);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.tail, "Expected %d, but got %d.", 0, t1_rb.tail);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.max_elements, "Expected %d, but got %d.", 0, t1_rb.max_elements);

    return failed_assertions;
}

static int32_t test_ring_buffer_insert_null_handle(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    uint8_t t1_buffer[rand() % 1024 + 1];
    uint8_t t1_data = 0xAA;
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {
        .buffer = t1_buffer, .buffer_size = sizeof(t1_buffer), .element_size = 1, .overwrite = false};

    t1_result = RING_BUFFER_Init(&t1_rb, t1_conf);
    t1_result = RING_BUFFER_Insert(NULL, &t1_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, t1_result,
                  "RING_BUFFER_Insert(%p, %p) -> Expected %d, but got %d.", NULL, &t1_data,
                  RING_BUFFER_STATUS_ERROR_INPUT_ARGS, t1_result);

    t1_result = RING_BUFFER_Insert(&t1_rb, NULL);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, t1_result,
                  "RING_BUFFER_Insert(%p, %p) -> Expected %d, but got %d.", &t1_rb, NULL,
                  RING_BUFFER_STATUS_ERROR_INPUT_ARGS, t1_result);

    return failed_assertions;
}

static int32_t test_ring_buffer_insert_full_buffer(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    uint8_t t1_buffer[1];
    uint8_t t1_data1 = 0xAA;
    uint8_t t1_data2 = 0xBB;
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {
        .buffer = t1_buffer,
        .buffer_size = sizeof(t1_buffer),
        .element_size = 1,
        .overwrite = false,
    };

    t1_result = RING_BUFFER_Init(&t1_rb, t1_conf);
    t1_result = RING_BUFFER_Insert(&t1_rb, &t1_data1);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, t1_result, "RING_BUFFER_Insert(%p, %p) -> Expected %d, but got %d.", &t1_rb,
                  &t1_data1, RING_BUFFER_STATUS_OK, t1_result);
    CTEST_ASSERT_EQ_MSG(1, t1_rb.count, "Expected %d, but got %d.", 1, t1_rb.count);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.head, "Expected %d, but got %d.", 0, t1_rb.head);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.tail, "Expected %d, but got %d.", 0, t1_rb.tail);

    t1_result = RING_BUFFER_Insert(&t1_rb, &t1_data2);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_BUFFER_FULL, t1_result,
                  "RING_BUFFER_Insert(%p, %p) -> Expected %d, but got %d.", &t1_rb, &t1_data2,
                  RING_BUFFER_STATUS_ERROR_BUFFER_FULL, t1_result);
    CTEST_ASSERT_EQ_MSG(1, t1_rb.count, "Expected %d, but got %d.", 1, t1_rb.count);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.head, "Expected %d, but got %d.", 0, t1_rb.head);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.tail, "Expected %d, but got %d.", 0, t1_rb.tail);

    return failed_assertions;
}

static int32_t test_ring_buffer_insert_overwrite(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    uint8_t t1_buffer[1];
    uint8_t t1_data1 = 0xAA;
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {
        .buffer = t1_buffer,
        .buffer_size = sizeof(t1_buffer),
        .element_size = 1,
        .overwrite = true,
    };

    t1_result = RING_BUFFER_Init(&t1_rb, t1_conf);
    t1_result = RING_BUFFER_Insert(&t1_rb, &t1_data1);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, t1_result, "RING_BUFFER_Insert(%p, %p) -> Expected %d, but got %d.", &t1_rb,
                  &t1_data1, RING_BUFFER_STATUS_OK, t1_result);

    t1_result = RING_BUFFER_Insert(&t1_rb, &t1_data1);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, t1_result, "RING_BUFFER_Insert(%p, %p) -> Expected %d, but got %d.", &t1_rb,
                  &t1_data1, RING_BUFFER_STATUS_OK, t1_result);
    CTEST_ASSERT_EQ_MSG(1, t1_rb.count, "Expected %d, but got %d.", 1, t1_rb.count);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.head, "Expected %d, but got %d.", 0, t1_rb.head);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.tail, "Expected %d, but got %d.", 0, t1_rb.tail);

    return failed_assertions;
}

static int32_t test_ring_buffer_insert_valid(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e t1_result;
    uint8_t t1_buffer[rand() % 1024 + 1];
    uint8_t t1_data = 0xAA;
    ring_buffer_t t1_rb;
    ring_buffer_conf_t t1_conf = {
        .buffer = t1_buffer, .buffer_size = sizeof(t1_buffer), .element_size = 1, .overwrite = false};

    t1_result = RING_BUFFER_Init(&t1_rb, t1_conf);
    t1_result = RING_BUFFER_Insert(&t1_rb, &t1_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, t1_result, "RING_BUFFER_Insert(%p, %p) -> Expected %d, but got %d.", &t1_rb,
                  &t1_data, RING_BUFFER_STATUS_OK, t1_result);
    CTEST_ASSERT_EQ_MSG(sizeof(t1_data), t1_rb.count, "Expected %d, but got %d.", sizeof(t1_data), t1_rb.count);
    CTEST_ASSERT_EQ_MSG(sizeof(t1_data), t1_rb.head, "Expected %d, but got %d.", sizeof(t1_data), t1_rb.head);
    CTEST_ASSERT_EQ_MSG(0, t1_rb.tail, "Expected %d, but got %d.", 0, t1_rb.tail);

    // @todo test also if data is still when writing at end of buffer and start of buffer

    return failed_assertions;
}

static int32_t test_ring_buffer_retrieve_null_handle(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[rand() % 1024 + 1];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    result = RING_BUFFER_Retrieve(NULL, &data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result,
                  "RING_BUFFER_Retrieve(%p, %p) -> Expected %d, but got %d.", NULL, &data,
                  RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result);

    result = RING_BUFFER_Retrieve(&rb, NULL);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result,
                  "RING_BUFFER_Retrieve(%p, %p) -> Expected %d, but got %d.", &rb, NULL,
                  RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_retrieve_valid(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[rand() % 1024 + 2];
    uint8_t data = 0xAA;
    uint8_t read_data;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);
    result = RING_BUFFER_Retrieve(&rb, &read_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_BUFFER_EMPTY, result,
                  "RING_BUFFER_Retrieve(%p, %p) -> Expected %d, but got %d.", &rb, &data,
                  RING_BUFFER_STATUS_ERROR_BUFFER_EMPTY, result);

    result = RING_BUFFER_Insert(&rb, &data);
    result = RING_BUFFER_Retrieve(&rb, &read_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Retrieve(%p, %p) -> Expected %d, but got %d.", &rb,
                  &read_data, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(1, rb.tail, "Expected %d, but got %d.", 1, rb.tail);

    result = RING_BUFFER_Retrieve(&rb, &read_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_BUFFER_EMPTY, result,
                  "RING_BUFFER_Retrieve(%p, %p) -> Expected %d, but got %d.", &rb, &data,
                  RING_BUFFER_STATUS_ERROR_BUFFER_EMPTY, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_peek_null_handle(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[rand() % 1024 + 1];
    uint8_t data = 0xAA;
    uint8_t read_data;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);
    result = RING_BUFFER_Peek(NULL, 0, &data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result,
                  "RING_BUFFER_Peek(%p, %d, %p) -> Expected %d, but got %d.", NULL, 0, &read_data,
                  RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result);

    result = RING_BUFFER_Peek(&rb, 0, NULL);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result,
                  "RING_BUFFER_Peek(%p, %d, %p) -> Expected %d, but got %d.", &rb, 0, NULL,
                  RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_peek_valid(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[2];
    uint8_t data1 = 0xAA;
    uint8_t data2 = 0xBB;
    uint8_t read_data;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);
    result = RING_BUFFER_Insert(&rb, &data1);
    result = RING_BUFFER_Peek(&rb, 0, &read_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Peek(%p, %d, %p) -> Expected %d, but got %d.", &rb, 0,
                  &read_data, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(data1, read_data, "Expected %d, but got %d.", data1, read_data);

    result = RING_BUFFER_Insert(&rb, &data2);
    result = RING_BUFFER_Peek(&rb, 1, &read_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Peek(%p, %d, %p) -> Expected %d, but got %d.", &rb, 0,
                  &read_data, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(data2, read_data, "Expected %d, but got %d.", data2, read_data);

    uint8_t buffer1[5];
    uint16_t data3 = 0x1122;
    uint16_t data4 = 0x3344;
    uint16_t read_data1;
    ring_buffer_t rb1;
    ring_buffer_conf_t conf1 = {
        .buffer = buffer1, .buffer_size = sizeof(buffer1), .element_size = sizeof(data3), .overwrite = true};

    result = RING_BUFFER_Init(&rb1, conf1);
    result = RING_BUFFER_Insert(&rb1, &data3);
    result = RING_BUFFER_Peek(&rb1, 0, &read_data1);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Peek(%p, %d, %p) -> Expected %d, but got %d.", &rb, 0,
                  &read_data, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(data3, read_data1, "Expected %d, but got %d.", data3, read_data1);

    result = RING_BUFFER_Insert(&rb1, &data4);
    result = RING_BUFFER_Peek(&rb1, 1, &read_data1);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Peek(%p, %d, %p) -> Expected %d, but got %d.", &rb, 1,
                  &read_data, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(data4, read_data1, "Expected %d, but got %d.", data4, read_data1);

    result = RING_BUFFER_Insert(&rb1, &data3);
    result = RING_BUFFER_Peek(&rb1, 1, &read_data1);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Peek(%p, %d, %p) -> Expected %d, but got %d.", &rb, 1,
                  &read_data, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(data3, read_data1, "Expected %d, but got %d.", data3, read_data1);

    return failed_assertions;
}

static int32_t test_ring_buffer_replace_null_handle(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[rand() % 1024 + 1];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);
    result = RING_BUFFER_Insert(&rb, &data);
    result = RING_BUFFER_Replace(NULL, 0, &data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result,
                  "RING_BUFFER_Replace(%p, %d, %p) -> Expected %d, but got %d.", NULL, 0, &data,
                  RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result);

    result = RING_BUFFER_Peek(&rb, 0, NULL);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result,
                  "RING_BUFFER_Replace(%p, %d, %p) -> Expected %d, but got %d.", &rb, 0, NULL,
                  RING_BUFFER_STATUS_ERROR_INPUT_ARGS, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_replace_empty_buffer(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Try to replace an element at index 0 in an empty buffer
    result = RING_BUFFER_Replace(&rb, 0, &data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_BUFFER_EMPTY, result,
                  "RING_BUFFER_Replace(%p, 0, %p) -> Expected %d, but got %d.", &rb, &data,
                  RING_BUFFER_STATUS_ERROR_BUFFER_EMPTY, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_replace_single_element(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    uint8_t new_data = 0xBB;
    uint8_t read_data;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);
    result = RING_BUFFER_Insert(&rb, &data);

    // Replace the first (and only) element
    result = RING_BUFFER_Replace(&rb, 0, &new_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Replace(%p, 0, %p) -> Expected %d, but got %d.", &rb,
                  &new_data, RING_BUFFER_STATUS_OK, result);

    // Verify the replaced data
    result = RING_BUFFER_Retrieve(&rb, &read_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Retrieve(%p, %p) -> Expected %d, but got %d.", &rb,
                  &read_data, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(new_data, read_data, "Expected %d, but got %d.", new_data, read_data);

    return failed_assertions;
}

static int32_t test_ring_buffer_replace_full_buffer(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    uint8_t new_data = 0xBB;
    uint8_t read_data;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert 5 elements into the buffer
    for (int i = 0; i < 5; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Replace the third element (index 2)
    result = RING_BUFFER_Replace(&rb, 2, &new_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Replace(%p, 2, %p) -> Expected %d, but got %d.", &rb,
                  &new_data, RING_BUFFER_STATUS_OK, result);

    // Verify the replaced data at index 2
    result = RING_BUFFER_Peek(&rb, 2, &read_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Peek(%p, 2, %p) -> Expected %d, but got %d.", &rb,
                  &read_data, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(new_data, read_data, "Expected %d, but got %d.", new_data, read_data);

    return failed_assertions;
}

static int32_t test_ring_buffer_replace_overwrite_enabled(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    uint8_t new_data = 0xBB;
    uint8_t read_data;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = true};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert 10 elements into the buffer (full buffer)
    for (int i = 0; i < 10; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Replace the first element (index 0)
    result = RING_BUFFER_Replace(&rb, 0, &new_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Replace(%p, 0, %p) -> Expected %d, but got %d.", &rb,
                  &new_data, RING_BUFFER_STATUS_OK, result);

    // Verify the replaced data at index 0
    result = RING_BUFFER_Peek(&rb, 0, &read_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Peek(%p, 0, %p) -> Expected %d, but got %d.", &rb,
                  &read_data, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(new_data, read_data, "Expected %d, but got %d.", new_data, read_data);

    return failed_assertions;
}

static int32_t test_ring_buffer_replace_valid(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    uint8_t new_data = 0xBB;
    uint8_t read_data;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert some elements into the buffer
    for (int i = 0; i < 5; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Replace the third element (index 2)
    result = RING_BUFFER_Replace(&rb, 2, &new_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Replace(%p, 2, %p) -> Expected %d, but got %d.", &rb,
                  &new_data, RING_BUFFER_STATUS_OK, result);

    // Verify the replaced data at index 2
    result = RING_BUFFER_Peek(&rb, 2, &read_data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Peek(%p, 2, %p) -> Expected %d, but got %d.", &rb,
                  &read_data, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(new_data, read_data, "Expected %d, but got %d.", new_data, read_data);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_empty_after_insert(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);
    result = RING_BUFFER_IsEmpty(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsEmpty(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    result = RING_BUFFER_Insert(&rb, &data);
    result = RING_BUFFER_IsEmpty(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR, result, "RING_BUFFER_IsEmpty(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_ERROR, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_empty_after_retrieve(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    uint8_t read_data;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert an element into the buffer
    result = RING_BUFFER_Insert(&rb, &data);

    // Retrieve the element from the buffer
    result = RING_BUFFER_Retrieve(&rb, &read_data);

    // Check if the buffer is empty after retrieving the element
    result = RING_BUFFER_IsEmpty(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsEmpty(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_empty_initialized(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Check if the buffer is empty after initialization
    result = RING_BUFFER_IsEmpty(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsEmpty(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_empty_with_overwrite(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    uint8_t new_data = 0xBB;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = true};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert elements into the buffer
    for (int i = 0; i < 10; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Check if buffer is full
    result = RING_BUFFER_IsEmpty(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR, result, "RING_BUFFER_IsEmpty(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_ERROR, result);

    // Overwrite the buffer
    result = RING_BUFFER_Insert(&rb, &new_data);

    // Check if buffer is still not empty after overwriting
    result = RING_BUFFER_IsEmpty(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR, result, "RING_BUFFER_IsEmpty(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_ERROR, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_empty_no_overwrite(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert elements into the buffer
    for (int i = 0; i < 10; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Try inserting another element into a full buffer (overwrite disabled)
    result = RING_BUFFER_Insert(&rb, &data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR_BUFFER_FULL, result,
                  "RING_BUFFER_Insert should fail when buffer is full and overwrite is disabled.");

    // Check if the buffer is still not empty
    result = RING_BUFFER_IsEmpty(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR, result, "RING_BUFFER_IsEmpty(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_ERROR, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_empty_valid(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Init(%p, %p) -> Expected %d, but got %d.", &rb, &conf,
                  RING_BUFFER_STATUS_OK, result);

    // Check if buffer is initially empty
    result = RING_BUFFER_IsEmpty(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsEmpty(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    // Insert an element and check again
    uint8_t data = 0xAA;
    result = RING_BUFFER_Insert(&rb, &data);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Insert(%p, %p) -> Expected %d, but got %d.", &rb, &data,
                  RING_BUFFER_STATUS_OK, result);

    result = RING_BUFFER_IsEmpty(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR, result, "RING_BUFFER_IsEmpty(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_ERROR, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_full_initialized(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Check if the buffer is not full after initialization
    result = RING_BUFFER_IsFull(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR, result, "RING_BUFFER_IsFull(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_ERROR, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_full_after_insert(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert elements into the buffer until it's full
    for (int i = 0; i < 10; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Check if the buffer is full
    result = RING_BUFFER_IsFull(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsFull(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_full_with_overwrite(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = true};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert elements into the buffer until it's full
    for (int i = 0; i < 10; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Check if the buffer is full
    result = RING_BUFFER_IsFull(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsFull(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    // Insert another element (overwrite should happen)
    result = RING_BUFFER_Insert(&rb, &data);

    // The buffer should still be full (overwrite behavior)
    result = RING_BUFFER_IsFull(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsFull(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_full_no_overwrite(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert elements into the buffer until it's full
    for (int i = 0; i < 10; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Check if the buffer is full
    result = RING_BUFFER_IsFull(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsFull(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    // Try inserting another element into a full buffer (overwrite disabled)
    result = RING_BUFFER_Insert(&rb, &data);
    result = RING_BUFFER_IsFull(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsFull(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_full_after_retrieve(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    uint8_t read_data;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert elements into the buffer until it's full
    for (int i = 0; i < 10; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Check if the buffer is full
    result = RING_BUFFER_IsFull(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsFull(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    // Retrieve one element from the buffer
    result = RING_BUFFER_Retrieve(&rb, &read_data);

    // The buffer should no longer be full
    result = RING_BUFFER_IsFull(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR, result, "RING_BUFFER_IsFull(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_ERROR, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_is_full_valid(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Init(%p, %p) -> Expected %d, but got %d.", &rb, &conf,
                  RING_BUFFER_STATUS_OK, result);

    // Check if buffer is initially empty
    result = RING_BUFFER_IsFull(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_ERROR, result, "RING_BUFFER_IsFull(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_ERROR, result);

    // Fill the buffer
    for (size_t i = 0; i < rb.max_elements; i++)
    {
        uint8_t data = (uint8_t)i;
        result = RING_BUFFER_Insert(&rb, &data);
        CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Insert(%p, %p) -> Expected %d, but got %d.", &rb,
                      &data, RING_BUFFER_STATUS_OK, result);
    }

    // Check if the buffer is full
    result = RING_BUFFER_IsFull(&rb);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_IsFull(%p) -> Expected %d, but got %d.", &rb,
                  RING_BUFFER_STATUS_OK, result);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_free_elements_empty(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    size_t free_elements;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Get the number of free elements in an empty buffer
    result = RING_BUFFER_GetFreeElements(&rb, &free_elements);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(10, free_elements, "Expected 10 free elements, but got %d.", free_elements);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_free_elements_after_insert(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    size_t free_elements;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert some elements into the buffer
    for (int i = 0; i < 5; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Get the number of free elements after some inserts
    result = RING_BUFFER_GetFreeElements(&rb, &free_elements);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(5, free_elements, "Expected 5 free elements, but got %d.", free_elements);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_free_elements_full(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    size_t free_elements;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert elements until the buffer is full
    for (int i = 0; i < 10; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Get the number of free elements when the buffer is full
    result = RING_BUFFER_GetFreeElements(&rb, &free_elements);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(0, free_elements, "Expected 0 free elements, but got %d.", free_elements);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_free_elements_with_overwrite(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    size_t free_elements;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = true};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert elements until the buffer is full
    for (int i = 0; i < 10; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Get the number of free elements with overwrite enabled
    result = RING_BUFFER_GetFreeElements(&rb, &free_elements);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(0, free_elements, "Expected 0 free elements, but got %d.", free_elements);

    // Insert one more element (overwrite should occur)
    result = RING_BUFFER_Insert(&rb, &data);

    // The free elements should still be 0 after overwrite
    result = RING_BUFFER_GetFreeElements(&rb, &free_elements);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(0, free_elements, "Expected 0 free elements, but got %d.", free_elements);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_free_elements_after_retrieve(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    size_t free_elements;
    uint8_t buffer[10];
    uint8_t data = 0xAA;
    uint8_t read_data;
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Insert elements into the buffer
    for (int i = 0; i < 5; i++)
    {
        result = RING_BUFFER_Insert(&rb, &data);
    }

    // Get the number of free elements
    result = RING_BUFFER_GetFreeElements(&rb, &free_elements);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(5, free_elements, "Expected 5 free elements, but got %d.", free_elements);

    // Retrieve one element from the buffer
    result = RING_BUFFER_Retrieve(&rb, &read_data);

    // The number of free elements should increase by 1
    result = RING_BUFFER_GetFreeElements(&rb, &free_elements);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(6, free_elements, "Expected 6 free elements, but got %d.", free_elements);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_free_elements_valid(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 1, .overwrite = false};
    size_t free_elements = 0;

    result = RING_BUFFER_Init(&rb, conf);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Init(%p, %p) -> Expected %d, but got %d.", &rb, &conf,
                  RING_BUFFER_STATUS_OK, result);

    // Check free elements initially
    result = RING_BUFFER_GetFreeElements(&rb, &free_elements);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_GetFreeElements(%p, %p) -> Expected %d, but got %d.", &rb,
                  &free_elements, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(rb.max_elements, free_elements, "Expected %d free elements, but got %d.", rb.max_elements,
                  free_elements);

    // Insert elements and check free space
    for (size_t i = 0; i < rb.max_elements / 2; i++)
    {
        uint8_t data = (uint8_t)i;
        result = RING_BUFFER_Insert(&rb, &data);
    }

    result = RING_BUFFER_GetFreeElements(&rb, &free_elements);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_GetFreeElements(%p, %p) -> Expected %d, but got %d.", &rb,
                  &free_elements, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(rb.max_elements / 2, free_elements, "Expected %d free elements, but got %d.", rb.max_elements / 2,
                  free_elements);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_element_size(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    size_t element_size;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 4, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Get the element size from the buffer
    result = RING_BUFFER_GetElementSize(&rb, &element_size);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(4, element_size, "Expected element size 4, but got %d.", element_size);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_element_size_different_size(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    size_t element_size;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 8, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Get the element size from the buffer
    result = RING_BUFFER_GetElementSize(&rb, &element_size);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(8, element_size, "Expected element size 8, but got %d.", element_size);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_element_size_initialized(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    size_t element_size;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 2, .overwrite = false};

    result = RING_BUFFER_Init(&rb, conf);

    // Get the element size from the buffer after initialization
    result = RING_BUFFER_GetElementSize(&rb, &element_size);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(2, element_size, "Expected element size 2, but got %d.", element_size);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_element_size_with_overwrite(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    size_t element_size;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 4, .overwrite = true};

    result = RING_BUFFER_Init(&rb, conf);

    // Get the element size from the buffer when overwrite is enabled
    result = RING_BUFFER_GetElementSize(&rb, &element_size);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "Expected RING_BUFFER_STATUS_OK, but got %d.", result);
    CTEST_ASSERT_EQ_MSG(4, element_size, "Expected element size 4, but got %d.", element_size);

    return failed_assertions;
}

static int32_t test_ring_buffer_get_element_size_valid(void)
{
    int32_t failed_assertions = 0;

    ring_buffer_status_e result;
    uint8_t buffer[10];
    ring_buffer_t rb;
    ring_buffer_conf_t conf = {.buffer = buffer, .buffer_size = sizeof(buffer), .element_size = 2, .overwrite = false};
    size_t element_size = 0;

    result = RING_BUFFER_Init(&rb, conf);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_Init(%p, %p) -> Expected %d, but got %d.", &rb, &conf,
                  RING_BUFFER_STATUS_OK, result);

    result = RING_BUFFER_GetElementSize(&rb, &element_size);
    CTEST_ASSERT_EQ_MSG(RING_BUFFER_STATUS_OK, result, "RING_BUFFER_GetElementSize(%p, %p) -> Expected %d, but got %d.", &rb,
                  &element_size, RING_BUFFER_STATUS_OK, result);
    CTEST_ASSERT_EQ_MSG(conf.element_size, element_size, "Expected element size %d, but got %d.", conf.element_size,
                  element_size);

    return failed_assertions;
}

// --- EOF -------------------------------------------------------------------------------------------------------------
