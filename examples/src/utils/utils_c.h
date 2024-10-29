// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the current system timestamp in milliseconds.
 *
 */
uint64_t ob_smpl_get_current_timestamp_ms();

/**
 * @brief Wait for key press.
 *
 * @param[in] timeout_ms The maximum time to wait for a key press in milliseconds. Set to 0 to wait indefinitely.
 *
 * @return char The key that was pressed.
 */
char ob_smpl_wait_for_key_press(uint32_t timeout_ms);

// Macro to check for error and exit program if there is one.
#define CHECK_OB_ERROR_EXIT(error)                                \
    if(*error) {                                                  \
        const char *error_message = ob_error_get_message(*error); \
        fprintf(stderr, "Error: %s\n", error_message);            \
        ob_delete_error(*error);                                  \
        *error = NULL;                                            \
        exit(-1);                                                 \
    }                                                             \
    *error = NULL;

#ifdef __cplusplus
}
#endif
