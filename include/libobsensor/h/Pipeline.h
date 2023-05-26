/**
 * @file Pipeline.h
 * @brief The SDK's advanced API can quickly implement functions such as switching streaming, frame synchronization, software filtering, etc., suitable for
 * applications, and the algorithm focuses on rgbd data stream scenarios. If you are on real-time or need to handle synchronization separately, align the scene.
 * Please use the interface of Device's Lower API.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ObTypes.h"

/**
 * @brief Create a pipeline object
 *
 * @param[out] error Log error messages
 * @return ob_pipeline* returns the pipeline object
 *
 */
ob_pipeline *ob_create_pipeline(ob_error **error);

 /**
 * @brief Using device objects to create pipeline objects
 *
 * @param[in] dev Device object used to create pipeline
 * @param[out] error  Log error messages
 * @return ob_pipeline* returns the pipeline object
 *
 */
ob_pipeline *ob_create_pipeline_with_device(ob_device *dev, ob_error **error);

 /**
 * @brief Use the playback file to create a pipeline object
 *
 * @param[in] file_name The playback file path used to create the pipeline
 * @param[out] error Log error messages
 * @return ob_pipeline* returns the pipeline object
 *
 */
ob_pipeline *ob_create_pipeline_with_playback_file(const char *file_name, ob_error **error);

/**
 * @brief Delete pipeline objects
 *
 * @param[in] pipeline  The pipeline object to be deleted
 * @param[out] error Log error messages
 *
 */
void ob_delete_pipeline(ob_pipeline *pipeline, ob_error **error);

/**
 * @brief Start the pipeline with default parameters
 *
 * @param[in] pipeline  pipeline object
 * @param[out] error Log error messages
 *
 */
void ob_pipeline_start(ob_pipeline *pipeline, ob_error **error);

/**
 * @brief Start the pipeline with configuration parameters
 *
 * @param[in] pipeline  pipeline object
 * @param[in] config Parameters to be configured
 * @param[out] error Log error messages
 *
 */
void ob_pipeline_start_with_config(ob_pipeline *pipeline, ob_config *config, ob_error **error);

/**
 * @brief Start the pipeline and set the frame collection data callback
 *
 * @param[in] pipeline pipeline object
 * @param[in] config  Parameters to be configured
 * @param[in] callback Trigger a callback when all frame data in the frameset arrives
 * @param[in] user_data  Pass in any user data and get it from the callback
 * @param[out] error  Log error messages
 *
 */
void ob_pipeline_start_with_callback(ob_pipeline *pipeline, ob_config *config, ob_frameset_callback callback, void *user_data, ob_error **error);

/**
 * @brief Stop pipeline
 *
 * @param[in] pipeline pipeline object
 * @param[out] error  Log error messages
 *
 */
void ob_pipeline_stop(ob_pipeline *pipeline, ob_error **error);

/**
 * @brief Get current pipeline parameters
 *
 * @param[in] pipeline pipeline object
 * @param[out] error Log error messages
 * @return ob_config*  returns pipeline parameters
 *
 */
ob_config *ob_pipeline_get_config(ob_pipeline *pipeline, ob_error **error);

/**
 * @brief Waiting for the return of a set of frames in the form of synchronous blocking
 *
 * @param[in] pipeline pipeline object
 * @param[in] timeout_ms Waiting timeout (ms)
 * @param[out] error Log error messages
 * @return ob_frame*  returns the waiting frameset, frameset is a special frame, you can use the frameset related interface to obtain the independent frame in
 * the set
 */
ob_frame *ob_pipeline_wait_for_frameset(ob_pipeline *pipeline, uint32_t timeout_ms, ob_error **error);

/**
 * @brief Get device from the pipeline
 *
 * @param[in] pipeline pipeline object
 * @param[out] error Log error messages
 * @return ob_device* returns the device object
 *
 */
ob_device *ob_pipeline_get_device(ob_pipeline *pipeline, ob_error **error);

/**
 * @brief Get playback object from pipeline
 *
 * @param[in] pipeline pipeline objet
 * @param[out] error Log error messages
 * @return ob_playback* returns the playback object
 *
 */
ob_playback *ob_pipeline_get_playback(ob_pipeline *pipeline, ob_error **error);

/**
 * @brief Get stream profile from the pipeline
 *
 * @param[in] pipeline pipeline object
 * @param[in] sensor_type sensor type, the sensor_type type supported by the camera can be obtained through the ob_device_get_sensor_list() interface
 * @param[out] profile_count The number of stream profiles obtained
 * @param[out] error Log error messages
 * @return ob_stream_profile_list* returns to the stream profile list
 *
 */
ob_stream_profile_list *ob_pipeline_get_stream_profile_list(ob_pipeline *pipeline, ob_sensor_type sensorType, ob_error **error);

/**
 * @brief Enable frame synchronization
 *
 * @param[in] pipeline pipeline object
 * @param[out] error Log error messages
 *
 */
void ob_pipeline_enable_frame_sync(ob_pipeline *pipeline, ob_error **error);

/**
 * @brief Disable frame synchronization
 *
 * @param[in] pipeline pipeline object
 * @param[out] error Log error messages
 *
 */
void ob_pipeline_disable_frame_sync(ob_pipeline *pipeline, ob_error **error);

/**
 * @brief Dynamically switch the corresponding config configuration
 *
 * @param[in] pipeline pipeline object
 * @param[in] config pipeline configuration
 * @param[out] error Log error messages
 *
 */
void ob_pipeline_switch_config(ob_pipeline *pipeline, ob_config *config, ob_error **error);
/**
 * @brief Get current camera parameters
 * @attention If D2C is enabled, it will return the camera parameters after D2C, if not, it will return to the default parameters
 *
 * @param[in] pipeline pipeline object
 * @param[out] error Log error messages
 * @return ob_camera_param returns camera internal parameters
 *
 */
ob_camera_param ob_pipeline_get_camera_param(ob_pipeline *pipeline, ob_error **error);

/**
 * @brief Returns a list of D2C-enabled depth sensor resolutions corresponding to the input color sensor resolution
 *
 * @param[in] pipeline pipeline object
 * @param[in] color_profile Input resolution of Color Sensor
 * @param[in] align_mode Input align mode
 * @param[out] error Log error messages
 * @return ob_stream_profile_list*
 *
 */
ob_stream_profile_list *ob_get_d2c_depth_profile_list(ob_pipeline *pipeline, ob_stream_profile *color_profile, ob_align_mode align_mode, ob_error **error);

/**
 * @brief Get valid area after D2C (DEPRECATED)
 *
 * @param[in] pipeline pipeline object
 * @param[in] distance working distance
 * @param[out] error Log error messages
 * @return ob_rect returns the area information valid after D2C at working distance
 *
 */
ob_rect ob_get_d2c_valid_area(ob_pipeline *pipeline, uint32_t distance, ob_error **error);

/**
 * @brief Get valid area between minimum distance and maximum distance after D2C
 *
 * @param[in] pipeline pipeline object
 * @param[in] minimum_distance minimum working distance
 * @param[in] maximum_distance maximum working distance
 * @param[out] error Log error messages
 * @return ob_rect returns the area information valid after D2C at working distance
 *
 */
ob_rect ob_get_d2c_range_valid_area(ob_pipeline *pipeline, uint32_t minimum_distance, uint32_t maximum_distance, ob_error **error);

/**
 * @brief Start recording
 *
 * @param[in] pipeline pipeline object
 * @param[in] file_name Recorded file path
 * @param[out] error Log error messages
 *
 */
void ob_pipeline_start_record(ob_pipeline *pipeline, const char *file_name, ob_error **error);

/**
 * @brief Turn off recording
 *
 * @param[in] pipeline pipeline object
 * @param[out] error Log error messages
 *
 */

void ob_pipeline_stop_record(ob_pipeline *pipeline, ob_error **error);

/**
 * @brief Create the pipeline configuration
 *
 * @param[out] error  Log error messages
 * @return ob_config* returns the configuration object
 *
 */
ob_config *ob_create_config(ob_error **error);

/**
 * @brief Delete the pipeline configuration
 *
 * @param[in] config Configuration to be deleted
 * @param[out] error  Log error messages
 *
 */
void ob_delete_config(ob_config *config, ob_error **error);

/**
 * @brief Configure the stream to be enabled
 *
 * @param[in] config  Configuration of pipeline
 * @param[in] profile The configuration of the stream to be opened
 * @param[out] error Log error messages
 *
 */
void ob_config_enable_stream(ob_config *config, ob_stream_profile *profile, ob_error **error);

/**
 * @brief Configure all streams to be enabled
 *
 * @param[in] config Configuration of pipeline
 * @param[out] error  Log error messages
 *
 */
void ob_config_enable_all_stream(ob_config *config, ob_error **error);

/**
 * @brief Configure the stream to be disabled
 *
 * @param[in] config Configuration of pipeline
 * @param[in] profile  The configuration of the stream to be closed
 * @param[out] error Log error messages
 *
 */
void ob_config_disable_stream(ob_config *config, ob_stream_type type, ob_error **error);

/**
 * @brief Configure all streams to be disabled
 *
 * @param[in] config Configuration of pipeline
 * @param[out] error Log error messages
 *
 */
void ob_config_disable_all_stream(ob_config *config, ob_error **error);

/**
 * @brief Set the alignment mode
 *
 * @param[in] config config object
 * @param[in] mode alignment mode
 * @param[out] error Log error messages
 *
 */
void ob_config_set_align_mode(ob_config *config, ob_align_mode mode, ob_error **error);

/**
 * @brief Whether the depth needs to be scaled after setting D2C
 *
 * @param[in] config config object
 * @param[in] enable Whether scaling is required
 * @param[out] error Log error messages
 *
 */
void ob_config_set_depth_scale_require(ob_config *config, bool enable, ob_error **error);

/**
 * @brief  Set the D2C target resolution, which is applicable to cases where the color stream is not enabled using the OrbbecSDK and the depth needs to be D2C
 * Note: When you use OrbbecSDK to enable the color stream, you also use this interface to set the D2C target resolution. The configuration of the enabled Color
 * stream is preferred for D2C
 *
 * @param[in] config config object
 * @param[in] d2c_target_width The D2C target has a wide resolution
 * @param[in] d2c_target_height The D2C targets has a high resolution
 * @param[out] error Log error messages
 *
 */
void ob_config_set_d2c_target_resolution(ob_config *config, uint32_t d2c_target_width, uint32_t d2c_target_height, ob_error **error);

/**
 * @brief Frame aggregation output mode
 * @brief The processing strategy when the FrameSet generated by the frame aggregation function does not contain the frames of all opened streams (which
 * can be caused by different frame rates of each stream, or by the loss of frames of one stream): drop directly or output to the user
 *
 * @param config config object
 * @param mode frame aggregation output mode（default mode is @ref OB_FRAME_AGGREGATE_OUTPUT_ANY_SITUATION）
 * @param error  Log error messages
 *
 */
void ob_config_set_frame_aggregate_output_mode(ob_config *config, ob_frame_aggregate_output_mode mode, ob_error **error);

#ifdef __cplusplus
}
#endif
