/**
 * @file Error.h
 * @brief Error handling related functions, mainly used to obtain error messages
 *
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ObTypes.h"

/**
 * @brief Get error status
 *
 * @param[in] error  Error object
 * @return ob_status returns error status
 *
 */
ob_status ob_error_status(ob_error *error);
/**
 * @brief Get error information
 *
 * @param[in] error  Error object
 * @return const char* returns an error message
 *
 */
const char *ob_error_message(ob_error *error);
/**
 * @brief Get error API function name
 *
 * @param[in] error Error object
 * @return const char* returns error function
 *
 */
const char *ob_error_function(ob_error *error);
/**
 * @brief Get error parameters
 *
 * @param[in] error  Error object
 * @return const char* returns the error parameter
 *
 */
const char *ob_error_args(ob_error *error);
/**
 * @brief Get error exception type
 *
 * @param[in] error Error object
 * @return ob_exception_type returns the error exception type
 *
 */
ob_exception_type ob_error_exception_type(ob_error *error);
/**
 * @brief Delete the error object
 *
 * @param[in] error The error object to delete
 *
 */
void ob_delete_error(ob_error *error);

#ifdef __cplusplus
}
#endif
