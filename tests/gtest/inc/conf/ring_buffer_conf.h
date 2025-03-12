/***********************************************************************************************************************
 *
 * @file        data_logger_conf.h
 * @brief       Component DATA-LOGGER configurations.
 * @author      Blaz Baskovc
 * @copyright   Copyright 2025 Blaz Baskovc
 * @date        2025-02-24
 *
 **********************************************************************************************************************/

#ifndef DATA_LOGGER_CONF_H
#define DATA_LOGGER_CONF_H

// C++ wrapper - Start
#ifdef __cplusplus
extern "C" {
#endif

// --- Data Logger Configurations --------------------------------------------------------------------------------------

#define RING_BUFFER_CONF_FREERTOS_USE false           /// Set to true to enable thread-safety mechanisms.
#define RING_BUFFER_CONF_TRACE_USE    true            /// Set to true to enable logging of buffer actions using TRACE.
#define RING_BUFFER_CONF_TRACE_LEVEL  TRACE_LEVEL_VER /// Configure trace level (if tracing is used).

// C++ wrapper - End
#ifdef __cplusplus
}
#endif

#endif /* DATA_LOGGER_CONF_H */

// --- EOF -------------------------------------------------------------------------------------------------------------
