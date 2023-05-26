 /**
 * @file Context.h
 * @brief Context is a management class that describes the runtime of the SDK and is responsible for resource application and release of the SDK.
 * Context has the ability to manage multiple devices. It is responsible for enumerating devices, monitoring device callbacks, and enabling multi device
 * synchronization.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ObTypes.h"

/**
 * @brief create context api
 *
 * @param[out] error record the error information
 * @return ob_context* return the context that created
 *
 */
ob_context *ob_create_context(ob_error **error);

/**
 * @brief create context with config
 *
 * @param[in] config_path Configure the path of the file, and return null if the default path is used.
 * @param[out] error Log error messages
 * @return ob_context* returns the context
 *
 */
ob_context *ob_create_context_with_config(const char *config_path, ob_error **error);

/**
 * @brief Delete context
 *
 * @param[in] context The context to delete
 * @param[out] error Log error messages
 *
 */
void ob_delete_context(ob_context *context, ob_error **error);

/**
 * @brief Get the list of enumerated devices
 *
 * @param[in] context Context
 * @param[out] error Log error messages
 * @return ob_device_list* return device list object
 *
 */

ob_device_list *ob_query_device_list(ob_context *context, ob_error **error);

/**
 * @brief Create a network device
 *
 * @param[in] context Context
 * @param[in] address Device IP address
 * @param[in] port Device port
 * @param[out] error Log error messages
 * @return[out] ob_device* Returns the device object
 *
 */
ob_device *ob_create_net_device(ob_context *context, const char *address, uint16_t port, ob_error **error);

/**
 * @brief Set device plug-in callback function
 * @attention The added and removed device list returned through the callback interface need to be released manually
 *
 * @param[in] context Context
 * @param[in] callback Callback triggered when the device is plugged and unplugged
 * @param[in] user_data You can pass in any user data and get it from the callback
 * @param[out] error Log error messages
 *
 */
void ob_set_device_changed_callback(ob_context *context, ob_device_changed_callback callback, void *user_data, ob_error **error);

/**
 * @brief Activate the multi-device synchronization function to synchronize the clock of the created device(the device needs to support this function)
 *
 * @param[in]  context Context
 * @param[in]  repeatInterval synchronization time interval (unit: ms; if repeatInterval=0, it means that it will only be synchronized once and will not be
 * executed regularly)
 * @param[out] error Log error messages
 *
 */
void ob_enable_multi_device_sync(ob_context *context, uint64_t repeatInterval, ob_error **error);

/**
 * @brief free idle memory from internal frame memory pool
 *
 * @param[in]  context Context

 * @param[out] error Log error messages
 *
 */
void ob_free_idle_memory(ob_context *context, ob_error **error);

 /**
 * @brief Set the global log level and this will affect both the log level output to the console and the log output to the file
 *
 * @param[in] severity Output log level
 * @param[out] error Log error messages
 *
 */
void ob_set_logger_severity(ob_log_severity severity, ob_error **error);

 /**
 * @brief Set output log to file
 *
 * @param[in] severity log level output to file
 * @param[in] directory The log file output path. If the path is empty, the existing settings will continue to be used (if the existing configuration is also
 * empty, the log will not be output to the file)
 * @param[out] error Log error messages
 *
 */
void ob_set_logger_to_file(ob_log_severity severity, const char *directory, ob_error **error);

 /**
 * @brief Set the output log to the console
 *
 * @param[in] log Log level
 * @param[out] error Log error messages
 *
 */
void ob_set_logger_to_console(ob_log_severity severity, ob_error **error);

 /**
 * @brief Load License
 *
 * @param[in] filePath license file path
 * @param[in] key decrypt key,"OB_DEFAULT_DECRYPT_KEY" can be used to represent the default key
 * @param[out] error Log error messages
 *
 */
void ob_load_license(const char *filePath, const char *key, ob_error **error);

 /**
 * @brief Load license from data
 *
 * @param[in] data license data
 * @param[in] dataLen license data len
 * @param[in] key decrypt key,"OB_DEFAULT_DECRYPT_KEY" can be used to represent the default key
 * @param[out] error Log error messages
 *
 */
void ob_load_license_from_data(const char *data, uint32_t dataLen, const char *key, ob_error **error);

#ifdef __cplusplus
}
#endif
