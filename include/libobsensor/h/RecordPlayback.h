/**
 * @file RecordPlayback.h
 * @brief For recording and playback functions
 *
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ObTypes.h"

/**
 * @brief Create a recorder for data recording
 *
 * @param[out] error log error messages
 * @return ob_recorder* returns the recorder object
 *
 */
ob_recorder *ob_create_recorder(ob_error **error);

/**
 * @brief Create a recorder for data recording
 *
 * @param dev The device object used to create the recorder
 * @param[out] error log error messages
 * @return ob_recorder* returns the recorder object
 *
 */
ob_recorder *ob_create_recorder_with_device(ob_device *dev, ob_error **error);

/**
 * @brief delete recorder object
 *
 * @param recorder recorder object
 * @param[out] error log error messages
 *
 */
void ob_delete_recorder(ob_recorder *recorder, ob_error **error);

/**
 * @brief start recording
 *
 * @param[in] recorder recorder object
 * @param[in] filename Recorded file name
 * @param[in] async Whether to record asynchronously
 * @param[out] error log error messages
 *
 */
void ob_recorder_start(ob_recorder *recorder, const char *filename, bool async, ob_error **error);

/**
 * @brief stop recording
 *
 * @param[in] recorder recorder object
 * @param[out] error log error messages
 *
 */
void ob_recorder_stop(ob_recorder *recorder, ob_error **error);

/**
 * @brief Write frame data to the recorder
 *
 * @param[in] recorder recorder object
 * @param[in] frame Write frame data
 * @param[out] error log error messages
 *
 */
void ob_recorder_write_frame(ob_recorder *recorder, ob_frame *frame, ob_error **error);

/**
 * @brief Create playback object
 * @param[in] filename Playback filename
 * @param[out] error log error messages
 * @return ob_playback* returns the playback object
 *
 */
ob_playback *ob_create_playback(const char *filename, ob_error **error);

/**
 * @brief delete playback object
 *
 * @param[in] playback playback object
 * @param[out] error log error messages
 *
 */
void ob_delete_playback(ob_playback *playback, ob_error **error);

/**
 * @brief Start playback, playback data is returned from the callback
 *
 * @param[in] playback 	playback object
 * @param[in] callback Callback for playback data
 * @param[in] user_data User data
 * @param[in] type Type of playback data
 * @param[out] error log error messages
 *
 */
void ob_playback_start(ob_playback *playback, ob_playback_callback callback, void *user_data, ob_media_type type, ob_error **error);

/**
 * @brief stop playback
 *
 * @param[in] playback 	playback object
 * @param[out] error log error messages
 *
 */
void ob_playback_stop(ob_playback *playback, ob_error **error);

/**
 * @brief Set playback state
 * @param[in] playback 	playback object
 * @param[in] callback playback status callback
 * @param[in] user_data User data
 * @param[out] error log error messages
 *
 */
void ob_set_playback_state_callback(ob_playback *playback, ob_media_state_callback callback, void *user_data, ob_error **error);

/**
 * @brief Get the device information in the recording file
 *
 * @param[in] playback 	playback object
 * @param[out] error log error messages
 * @return ob_device_info* returns device information
 *
 */
ob_device_info *ob_playback_get_device_info(ob_playback *playback, ob_error **error);

/**
 * @brief Get the intrinsic and extrinsic parameter information in the recording file
 *
 * @param[in] playback playback object
 * @param[out] error log error messages
 * @return ob_camera_param returns internal and external parameter information
 *
 */
ob_camera_param ob_playback_get_camera_param(ob_playback *playback, ob_error **error);

#ifdef __cplusplus
}
#endif
