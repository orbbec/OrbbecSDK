 /**
 * @file Device.h
 * @brief Device-related functions, including operations such as obtaining and creating a device, setting and obtaining device property, and obtaining sensors
 *
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ObTypes.h"
#include "Property.h"

/**
 * @brief Get the number of devices
 *
 * @param[in] list Device list object
 * @param[out] error Log error messages
 * @return uint32_t returns the number of devices
 *
 */
uint32_t ob_device_list_device_count(ob_device_list *list, ob_error **error);

/**
 * @brief Get device name (DEPRECATED)
 *
 * @param[in] list Device list object
 * @param[in] index Device index
 * @param[out] error Log error messages
 * @return const char* returns device name
 *
 */
const char *ob_device_list_get_device_name(ob_device_list *list, uint32_t index, ob_error **error);

/**
 * @brief Get the pid of the specified device
 *
 * @param[in] list Device list object
 * @param[in] index Device index
 * @param[out] error Log error messages
 * @return int returns the device pid
 *
 */
int ob_device_list_get_device_pid(ob_device_list *list, uint32_t index, ob_error **error);

/**
 * @brief Get the vid of the specified device
 *
 * @param[in] list Device list object
 * @param[in] index Device index
 * @param[out] error Log error messages
 * @return int return device vid
 *
 */
int ob_device_list_get_device_vid(ob_device_list *list, uint32_t index, ob_error **error);

/**
 * @brief Get the uid of the specified device
 *
 * @param[in] list Device list object
 * @param[in] index Device index
 * @param[out] error Log error messages
 * @return const char* returns the device uid
 *
 */
const char *ob_device_list_get_device_uid(ob_device_list *list, uint32_t index, ob_error **error);

/**
 * @brief Get the serial number of the specified device
 *
 * @param[in] list Device list object
 * @param[in] index Device index
 * @param[out] error Log error messages
 * @return const char* returns the device uid
 *
 */
const char *ob_device_list_get_device_serial_number(ob_device_list *list, uint32_t index, ob_error **error);

/**
 * @brief Create device
 * @attention If the device has been acquired and created elsewhere, repeated acquisitions will return an error
 *
 * @param[in] list Device list object
 * @param index Creat index of the device
 * @param[out] error Log error messages
 * @return ob_device* returns the created device
 *
 */
ob_device *ob_device_list_get_device(ob_device_list *list, uint32_t index, ob_error **error);

/**
 * @brief Create device
 * @attention If the device has been acquired and created elsewhere, repeated acquisitions will return an error
 *
 * @param[in] list Device list object
 * @param index Creat the serial number of the device
 * @param[out] error Log error messages
 * @return ob_device* returns the created device
 *
 */
ob_device *ob_device_list_get_device_by_serial_number(ob_device_list *list, const char *serial_number, ob_error **error);

 /**
 * @brief Create device
 * @attention If the device has been acquired and created elsewhere, repeated acquisitions will return an error
 *
 * @param[in] list Device list object
 * @param index Creat the uid of the device
 * @param[out] error Log error messages
 * @return ob_device* returns the created device
 *
 */
ob_device *ob_device_list_get_device_by_uid(ob_device_list *list, const char *uid, ob_error **error);

 /**
 * @brief Delete device
 *
 * @param[in] device Device to be deleted
 * @param[out] error Log error messages
 *
 */
void ob_delete_device(ob_device *device, ob_error **error);

 /**
 * @brief Delete device information
 *
 * @param[in] info Device information to be deleted
 * @param[out] error Log error messages
 *
 */
void ob_delete_device_info(ob_device_info *info, ob_error **error);

 /**
 * @brief Delete device list
 *
 * @param[in] list Device list object to be deleted
 * @param[out] error Log error messages
 *
 */
void ob_delete_device_list(ob_device_list *list, ob_error **error);

/**
 * @brief Get device information
 *
 * @param[in] device Device to obtain information
 * @param[out] error Log error messages
 * @return ob_device_info* returns device information
 *
 */
ob_device_info *ob_device_get_device_info(ob_device *device, ob_error **error);
/**
 * @brief List all sensors
 *
 * @param[in] device Device object
 * @param[out] error Log error messages
 * @return ob_sensor_list* returns the list of all sensors
 *
 */
ob_sensor_list *ob_device_get_sensor_list(ob_device *device, ob_error **error);
/**
 * @brief Get the device's sensors
 *
 * @param[in] device Device object
 * @param[in] type Type of sensor to get
 * @param[out] error Log error messages
 * @return ob_sensor* returns the acquired sensor
 *
 */
ob_sensor *ob_device_get_sensor(ob_device *device, ob_sensor_type type, ob_error **error);

 /**
 * @brief Set int type of device property
 *
 * @param[in] device Device object
 * @param[in] property_id The id of the property to be set
 * @param[in] property Property value to be set
 * @param[out] error Log error messages
 *
 */
void ob_device_set_int_property(ob_device *device, ob_property_id property_id, int32_t property, ob_error **error);

 /**
 * @brief Get int type of device property
 *
 * @param[in] device Device object
 * @param[in] property_id Property id
 * @param[out] error Log error messages
 * @return int32_t returns the property value
 *
 */
int32_t ob_device_get_int_property(ob_device *device, ob_property_id property_id, ob_error **error);

 /**
 * @brief Set float type of device property
 *
 * @param[in] device Device object
 * @param[in] property_id The id of the property to be set
 * @param[in] property Property value to be set
 * @param[out] error Log error messages
 *
 */
void ob_device_set_float_property(ob_device *device, ob_property_id property_id, float property, ob_error **error);

 /**
 * @brief Get float type of device property
 *
 * @param[in] device Device object
 * @param[in] property_id Property id
 * @param[out] error Log error messages
 * @return int32_t returns the property value
 *
 */
float ob_device_get_float_property(ob_device *device, ob_property_id property_id, ob_error **error);

 /**
 * @brief Set bool type of device property
 *
 * @param[in] device Device object
 * @param[in] property_id The id of the property to be set
 * @param[in] property Property value to be set
 * @param[out] error Log error messages
 *
 */
void ob_device_set_bool_property(ob_device *device, ob_property_id property_id, bool property, ob_error **error);

 /**
 * @brief Get bool type of device property
 *
 * @param[in] device Device object
 * @param[in] property_id Property id
 * @param[out] error Log error messages
 * @return bool returns the property value
 *
 */
bool ob_device_get_bool_property(ob_device *device, ob_property_id property_id, ob_error **error);

 /**
 * @brief Set structured type
 *
 * @param[in] device Device object
 * @param[in] property_id The id of the property to be set
 * @param[in] data Property data to be set
 * @param[in] data_size The size of the property to be set
 * @param[out] error Log error messages
 *
 */
void ob_device_set_structured_data(ob_device *device, ob_property_id property_id, const void *data, uint32_t data_size, ob_error **error);

 /**
 * @brief Get structured type
 *
 * @param[in] device Device object
 * @param[in] property_id Id of the property
 * @param[out] data Property data obtained
 * @param[out] data_size Get the size of the property
 * @param[out] error Log error messages
 *
 */
void ob_device_get_structured_data(ob_device *device, ob_property_id property_id, void *data, uint32_t *data_size, ob_error **error);

 /**
 * @brief Set property struct data.
 *
 * @param[in] device Device object
 * @param[in] property_id Id of the property
 * @param[in] data_bundle Target data to set
 * @param[in] Set data callback
 * @param[in] user_data User-defined data will be returned in the callback
 * @param[out] error Log error messages
 *
 */
void ob_device_set_structured_data_ext(ob_device *device, ob_property_id property_id, ob_data_bundle *data_bundle, ob_set_data_callback cb, void *user_data,
                                       ob_error **error);

 /**
 * @brief Get property struct data.
 * @param[in] device Device object
 * @param[in] property_id Id of the property
 * @param[out] error Log error messages
 * @return ob_data_dundle, NOTE: ob_data_dundle must free by ob_delete_data_bundle() because it come from OrbbecSDK's API
 *
 */
ob_data_bundle *ob_device_get_structured_data_ext(ob_device *device, ob_property_id property_id, ob_error **error);

 /**
 * @brief Set raw data type of device property
 *
 * @param[in] device Device object
 * @param[in] property_id The id of the property to be set
 * @param[in] data Property data to be set
 * @param[in] data_size The size of the property to be set
 * @param[in] cb Set data callback
 * @param[in] async Whether to execute asynchronously
 * @param[in] user_data User-defined data will be returned in the callback
 * @param[out] error Log error messages
 *
 */
void ob_device_set_raw_data(ob_device *device, ob_property_id property_id, void *data, uint32_t data_size, ob_set_data_callback cb, bool async, void *user_data,
                            ob_error **error);

 /**
 * @brief Get raw data type of device property
 *
 * @param[in] device Device object
 * @param[in] property_id Property id
 * @param[in] cb Get data callback
 * @param[in] async Whether to execute asynchronously
 * @param[in] user_data User-defined data will be returned in the callback
 * @param[out] error Log error messages
 *
 */
void ob_device_get_raw_data(ob_device *device, ob_property_id property_id, ob_get_data_callback cb, bool async, void *user_data, ob_error **error);

 /**
 * @brief Get the property protocol version
 *
 * @param[in] device Device object
 * @param[out] error Log error messages
 * @return ob_protocol_version
 */
ob_protocol_version ob_device_get_protocol_version(ob_device *device, ob_error **error);

 /**
 * @brief Get cmdVersion of property
 *
 * @param[in] device Device object
 * @param[in] propertyId Property id
 * @param[out] error Log error messages
 * @return ob_cmd_version
 *
 */
ob_cmd_version ob_device_get_cmd_version(ob_device *device, ob_property_id property_id, ob_error **error);

/**
 * @brief Get supported device properties count
 *
 * @param[in] device Device object
 * @param[out] error Log error messages
 * @return uint32_t returns the number of properties supported by the device
 *
 */
uint32_t ob_device_get_supported_property_count(ob_device *device, ob_error **error);

 /**
 * @brief Get supported device properties
 *
 * @param[in] device Device object
 * @param[in] index Property index
 * @param[out] error Log error messages
 * @return ob_global_unified_property_item returns the type of property supported by the device
 *
 */
ob_property_item ob_device_get_supported_property(ob_device *device, uint32_t index, ob_error **error);

/**
 * @brief Check if device property permission is supported
 *
 * @param[in] device Device object
 * @param[in] property_id Property id
 * @param[in] permission The type of permission that needs to be interpreted
 * @param[out] error Log error messages
 * @return bool returns whether it is supported
 *
 */
bool ob_device_is_property_supported(ob_device *device, ob_property_id property_id, ob_permission_type permission, ob_error **error);

 /**
 * @brief Get int type of device property range
 *
 * @param[in] device Device object
 * @param[in] property_id Property id
 * @param[out] error Log error messages
 * @return ob_int_property_range returns the property range
 *
 */
ob_int_property_range ob_device_get_int_property_range(ob_device *device, ob_property_id property_id, ob_error **error);

 /**
 * @brief Get float type of device property range
 *
 * @param[in] device Device object
 * @param[in] property_id Property id
 * @param[out] error Log error messages
 * @return ob_float_property_range returns the property range
 *
 */
ob_float_property_range ob_device_get_float_property_range(ob_device *device, ob_property_id property_id, ob_error **error);

 /**
 * @brief Get the bool type of device property range
 *
 * @param[in] device Device object
 * @param[in] property_id Property id
 * @param[out] error Log error messages
 * @return ob_bool_property_range returns the property range
 *
 */
ob_bool_property_range ob_device_get_bool_property_range(ob_device *device, ob_property_id property_id, ob_error **error);

 /**
 * @brief ahb write register
 *
 * @param[in] device Device object
 * @param reg Register to be written
 * @param mask Mask
 * @param value The value to be written
 * @param[out] error Log error messages
 *
 */
void ob_device_write_ahb(ob_device *device, uint32_t reg, uint32_t mask, uint32_t value, ob_error **error);

 /**
 * @brief Read ahb register
 *
 * @param[in] device Device object
 * @param reg Register to be read
 * @param mask Mask
 * @param value The value to be read
 * @param[out] error Log error messages
 *
 */
void ob_device_read_ahb(ob_device *device, uint32_t reg, uint32_t mask, uint32_t *value, ob_error **error);

 /**
 * @brief Write i2c register
 *
 * @param[in] device Device object
 * @param module_id I2c module id to be written
 * @param reg Register to be written
 * @param mask Mask
 * @param value The value to be written
 * @param[out] error Log error messages
 *
 */
void ob_device_write_i2c(ob_device *device, uint32_t module_id, uint32_t reg, uint32_t mask, uint32_t value, ob_error **error);

 /**
 * @brief Read i2c register
 *
 * @param[in] device Device object
 * @param module_id The id of the i2c module to be read
 * @param reg Register to be read
 * @param mask Mask
 * @param value The value to be read
 * @param[out] error Log error messages
 *
 */
void ob_device_read_i2c(ob_device *device, uint32_t module_id, uint32_t reg, uint32_t mask, uint32_t *value, ob_error **error);

 /**
 * @brief Set the properties of writing to Flash [Asynchronous Callback]
 *
 * @param[in] device Device object
 * @param offset flash offset address
 * @param data Property data to be written
 * @param data_size The size of the property to be written
 * @param cb Set data callback
 * @param[in] async Whether to execute asynchronously
 * @param[in] user_data User-defined data will be returned in the callback
 * @param[out] error Log error messages
 *
 */
void ob_device_write_flash(ob_device *device, uint32_t offset, const void *data, uint32_t data_size, ob_set_data_callback cb, bool async, void *user_data,
                           ob_error **error);

 /**
 * @brief Read Flash properties [asynchronous callback]
 *
 * @param[in] device Device object
 * @param offset flash offset address
 * @param data_size The size of the data to be read
 * @param cb Read flash data and progress callback
 * @param[in] async Whether to execute asynchronously
 * @param[in] user_data User-defined data will be returned in the callback
 * @param[out] error Log error messages
 *
 */
void ob_device_read_flash(ob_device *device, uint32_t offset, uint32_t data_size, ob_get_data_callback cb, bool async, void *user_data, ob_error **error);

 /**
 * @brief Synchronize the device time (time to the device, synchronize the local system time to the device)
 *
 * @param[in] device Device object
 * @param[out] uint64_t Command round trip time (rtt)
 * @param[out] error Log error messages
 *
 */
uint64_t ob_device_sync_device_time(ob_device *device, ob_error **error);

 /**
 * @brief Device firmware upgrade
 *
 * @param[in] device Device object
 * @param[in] pathFirmware path
 * @param[in] callback Firmware upgrade progress callback
 * @param[in] async Whether to execute asynchronously
 * @param[in] user_data User-defined data will be returned in the callback
 * @param[out] error Log error messages
 *
 */
void ob_device_upgrade(ob_device *device, const char *path, ob_device_upgrade_callback callback, bool async, void *user_data, ob_error **error);

 /**
 * @brief Get current device status
 * @param[out] error Log error messages
 * @return ob_device_state device state information
 *
 */
ob_device_state ob_device_get_device_state(ob_device *device, ob_error **error);

 /**
 * @brief Monitor device state change
 *
 * @param[in] device Device object
 * @param[in] callback Callback when the device status changes
 * @param[in] user_data User-defined data will be returned in the callback
 * @param[out] error Log error messages
 *
 */
void ob_device_state_changed(ob_device *device, ob_device_state_callback callback, void *user_data, ob_error **error);

/**
 * @brief Send files to the specified path of the device
 *
 * @param[in] device Device object
 * @param[in] file_path Source file path
 * @param[in] dst_path Specify the device-side file acceptance path
 * @param[in] callback File sending progress callback
 * @param[in] async Whether to execute asynchronously
 * @param[in] user_data User-defined data will be returned in the callback
 * @param[out] error Log error messages
 *
 */
void ob_device_send_file_to_destination(ob_device *device, const char *file_path, const char *dst_path, ob_file_send_callback callback, bool async,
                                        void *user_data, ob_error **error);

/**
 * @brief Verify device authorization code
 *
 * @param[in] device Device object
 * @param[in] auth_code Authorization code
 * @param[out] error Log error messages
 * @return bool whether the activation is successful
 *
 */
bool ob_device_activate_authorization(ob_device *device, const char *auth_code, ob_error **error);
/**
 * @brief Write device authorization code
 *
 * @param[in] device Device object
 * @param[in] auth_code Authorization code
 * @param[out] error Log error messages
 *
 */

void ob_device_write_authorization_code(ob_device *device, const char *auth_code, ob_error **error);

/**
 * @brief Get the original parameter list of camera calibration saved in the device. The parameters in the list do not correspond to the current open-stream
 * configuration. You need to select the parameters according to the actual situation, and may need to do scaling, mirroring and other processing.
 * Non-professional users are recommended to use the ob_pipeline_get_camera_param() interface.
 * @param[in] device Device object
 * @param[out] error Log error messages
 *
 * @return ob_camera_param_list camera parameter list
 *
 */
ob_camera_param_list *ob_device_get_calibration_camera_param_list(ob_device *device, ob_error **error);

/**
 * @brief Get current depth work mode
 * @param[in] device Device object
 * @param[out] error Log error messages
 * @return ob_depth_work_mode Current depth work mode
 *
 */
ob_depth_work_mode ob_device_get_current_depth_work_mode(ob_device *device, ob_error **error);

/**
 * @brief Switch depth work mode by ob_depth_work_mode. Prefer invoke ob_device_switch_depth_work_mode_by_name to switch depth mode
 *        when known the complete name of depth work mode.
 * @param[in] device Device object
 * @param[in] work_mode Depth work mode come from ob_depth_work_mode_list which return by ob_device_get_depth_work_mode_list
 * @param[out] error Log error messages
 * @return Return switch result. OB_STATUS_OK: success, other failed.
 *
 */
ob_status ob_device_switch_depth_work_mode(ob_device *device, const ob_depth_work_mode *work_mode, ob_error **error);

/**
 * @brief Switch depth work mode by work mode name.
 * @param[in] device Device object
 * @param[in] mode name Depth work mode name which equals to ob_depth_work_mode.name
 * @param[out] error Log error messages
 *
 * @return Return switch result. OB_STATUS_OK: success, other failed.
 *
 */
ob_status ob_device_switch_depth_work_mode_by_name(ob_device *device, const char *mode_name, ob_error **error);

/**
 * @brief Request support depth work mode list
 * @param[in] device Device object
 * @param[out] error Log error messages
 * @return ob_depth_work_mode_list list of ob_depth_work_mode
 *
 */

ob_depth_work_mode_list *ob_device_get_depth_work_mode_list(ob_device *device, ob_error **error);

/**
 * @brief Device reboot
 * @attention The device will be disconnected and reconnected. After the device is disconnected, the interface access to the device handle may be abnormal.
 * Please use the ob_delete_device interface to delete the handle directly. After the device is reconnected, it can be obtained again.
 *
 * @param[in] device Device object
 * @param[out] error Log error messages
 *
 */
void ob_device_reboot(ob_device *device, ob_error **error);

/**
 * @brief Gets the current device synchronization configuration
 * @brief Device synchronization: including exposure synchronization function and multi-camera synchronization function of different sensors within a single
 * machine
 *
 * @param[in] device Device object
 * @param[out] error Log error messages
 * @return ob_device_sync_config Returns the device synchronization configuration
 *
 */
ob_device_sync_config ob_device_get_sync_config(ob_device *device, ob_error **error);

/**
 * @brief Set the device synchronization configuration
 * @brief Used to configure the exposure synchronization function and multi-camera synchronization function of different sensors in a single machine
 *
 * @attention Calling this function will directly write the configuration to the device Flash, and it will still take effect after the device restarts. To avoid
 * affecting the Flash lifespan, do not update the configuration frequently.
 *
 * @param[in] device Device object
 * @param[out] device_sync_config Device synchronization configuration
 * @param[out] error Log error messages
 *
 */
void ob_device_set_sync_config(ob_device *device, ob_device_sync_config device_sync_config, ob_error **error);

/**
 * @brief Get device name
 *
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return const char* returns the device name
 *
 */
const char *ob_device_info_name(ob_device_info *info, ob_error **error);

/**
 * @brief Get device pid

 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return int returns the device pid
 *
 */
int ob_device_info_pid(ob_device_info *info, ob_error **error);

/**
 * @brief Get device vid
 *
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return int returns device vid
 *
 */
int ob_device_info_vid(ob_device_info *info, ob_error **error);

/**
 * @brief Get device uid
 *
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return const char* returns device uid
 *
 */
const char *ob_device_info_uid(ob_device_info *info, ob_error **error);

/**
 * @brief Get device serial number
 *
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return const char* returns device serial number
 *
 */
const char *ob_device_info_serial_number(ob_device_info *info, ob_error **error);

/**
 * @brief Get the firmware version number
 *
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return int returns the firmware version number
 *
 */
const char *ob_device_info_firmware_version(ob_device_info *info, ob_error **error);

/**
 * @brief Get usb connection type (DEPRECATED)
 *
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return char* returns usb connection type
 *
 */
const char *ob_device_info_usb_type(ob_device_info *info, ob_error **error);

/**
 * @brief Get device connection type
 *
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return char* returns connection type
 *
 */
const char *ob_device_info_connection_type(ob_device_info *info, ob_error **error);

/**
 * @brief Get hardware version number
 *
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return const char* returns hardware version number
 *
 */
const char *ob_device_info_hardware_version(ob_device_info *info, ob_error **error);

/**
 * @brief Get the minimum SDK version number supported by the device
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return const char* returns the minimum SDK version number supported by the device
 *
 */
const char *ob_device_info_supported_min_sdk_version(ob_device_info *info, ob_error **error);

/**
 * @brief Get chip name
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return const char* returns asicName
 *
 */
const char *ob_device_info_asicName(ob_device_info *info, ob_error **error);

/**
 * @brief Get device type
 * @param[in] info Device Information
 * @param[out] error Log error messages
 * @return ob_device_type returns device type
 *
 */
ob_device_type ob_device_info_device_type(ob_device_info *info, ob_error **error);

/**
 * @brief Get the number of camera parameter lists
 *
 * @param param_list Camera parameter list
 * @param error Log error messages
 * @return uint32_t returns the number of lists
 *
 */
uint32_t ob_camera_param_list_count(ob_camera_param_list *param_list, ob_error **error);

/**
 * @brief Get camera parameters from camera parameter list
 *
 * @param param_list Camera parameter list
 * @param index Parameter index
 * @param error Log error messages
 * @return ob_camera_param returns the camera parameters. Since it returns the structure object directly, there is no need to provide a delete interface.
 *
 */
ob_camera_param ob_camera_param_list_get_param(ob_camera_param_list *param_list, uint32_t index, ob_error **error);

/**
 * @brief Delete camera parameter list
 *
 * @param param_list Camera parameter list
 * @param error Log error messages
 *
 */
void ob_delete_camera_param_list(ob_camera_param_list *param_list, ob_error **error);

/**
 * @brief Get the depth work mode count that ob_depth_work_mode_list hold
 * @param[in] work_mode_list data struct contain list of ob_depth_work_mode
 * @param[out] error Log error messages
 * @return The total number contain in ob_depth_work_mode_list
 *
 */
uint32_t ob_depth_work_mode_list_count(ob_depth_work_mode_list *work_mode_list, ob_error **error);

/**
 * @brief Get the index target of ob_depth_work_mode from work_mode_list
 * @param[in] work_mode_list data struct contain list of ob_depth_work_mode
 * @param[in] index index of target ob_depth_work_mode
 * @param[out] error Log error messages
 * @return ob_depth_work_mode
 *
 */
ob_depth_work_mode ob_depth_work_mode_list_get_item(ob_depth_work_mode_list *work_mode_list, uint32_t index, ob_error **error);

/**
 * @brief Free resource of ob_depth_work_mode_list
 * @param[in] work_mode_list data struct contain list of ob_depth_work_mode
 * @param[out] error Log error messages
 *
 */
void ob_delete_depth_work_mode_list(ob_depth_work_mode_list *work_mode_list, ob_error **error);

/**
 * @brief Free resource of data_bundle which come from OrbbecSDK's API
 * @param data_bundle Data bundle
 * @param[out] error Log error messages
 *
 */
void ob_delete_data_bundle(ob_data_bundle *data_bundle, ob_error **error);

#ifdef __cplusplus
}
#endif
