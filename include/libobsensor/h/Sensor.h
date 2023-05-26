/**
 * @file Sensor.h
 * @brief Sensor related types, used to obtain stream configuration, open and close streams, set and get sensor properties.
 *
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ObTypes.h"

/**
 * @brief Get sensor type
 *
 * @param[in] sensor Sensor object
 * @param[out] error Log error messages
 * @return ob_sensor_type returns the sensor type
 *
 */
ob_sensor_type ob_sensor_get_type(ob_sensor *sensor, ob_error **error);

/**
 * @brief Get all supported stream profiles
 *
 * @param[in] sensor Sensor object
 * @param[out] error Log error messages
 * @return ob_stream_profile_list* returns a list of stream profiles
 *
 */
ob_stream_profile_list *ob_sensor_get_stream_profile_list(ob_sensor *sensor, ob_error **error);

/**
 * @brief Open the current sensor, and set the callback data frame
 *
 * @param[in] sensor Sensor object
 * @param[in] profile Stream configuration information
 * @param[in] callback Callback function triggered when frame data arrives
 * @param[in] user_data  pass in any user data and get it from the callback
 * @param[out] error Log error messages
 *
 */
void ob_sensor_start(ob_sensor *sensor, ob_stream_profile *profile, ob_frame_callback callback, void *user_data, ob_error **error);

/**
 * @brief Stop sensor stream
 *
 * @param[in] sensor sensor object
 * @param[out] error Log error messages
 *
 */
void ob_sensor_stop(ob_sensor *sensor, ob_error **error);

/**
 * @brief Dynamically switch resolutions
 *
 * @param[in] sensor  	Sensor object
 * @param[in] profile Stream configuration information
 * @param[out] error Log error messages
 *
 */
void ob_sensor_switch_profile(ob_sensor *sensor, ob_stream_profile *profile, ob_error **error);

/**
 * @brief Delete sensor object list
 *
 * @param[in] sensor_list List of sensor objects to delete
 * @param[out] error Log error messages
 *
 */
void ob_delete_sensor_list(ob_sensor_list *sensor_list, ob_error **error);

/**
 * @brief Get the number of sensors in the sensor list
 *
 * @param[in] sensor_list  List of sensor objects to delete
 * @param[in] count Number of sensors
 * @param[out] error Log error messages
 *
 */
uint32_t ob_sensor_list_get_sensor_count(ob_sensor_list *sensor_list, ob_error **error);

/**
 * @brief Get the sensor types
 *
 * @param index Sensor index
 * @return OBSensorType returns the Sensor type
 *
 */
ob_sensor_type ob_sensor_list_get_sensor_type(ob_sensor_list *sensor_list, uint32_t index, ob_error **error);

/**
 * @brief Get sensor by sensor type
 *
 * @param sensor_list Get the returned sensor list through device
 * @param sensorType  Sensor type to be obtained
 * @return  Return the Sensor pointer, if the specified type of Sensor does not exist, it will return null
 *
 */
ob_sensor *ob_sensor_list_get_sensor_by_type(ob_sensor_list *sensor_list, ob_sensor_type sensorType, ob_error **error);

/**
 * @brief Get Sensor by index number
 *
 * @param sensor_list Get the returned sensor list through device
 * @param index To create a device cable, the range is [0, count-1], if the index exceeds the range, an exception will be thrown
 * @return std::shared_ptr<Sensor> returns the Sensor object
 *
 */
ob_sensor *ob_sensor_list_get_sensor(ob_sensor_list *sensor_list, uint32_t index, ob_error **error);

/**
 * @brief Delete sensor object
 *
 * @param[in] sensor The sensor object to delete
 * @param[out] error Log error messages
 *
 */
void ob_delete_sensor(ob_sensor *sensor, ob_error **error);

#ifdef __cplusplus
}
#endif
