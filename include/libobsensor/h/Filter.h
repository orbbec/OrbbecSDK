/**
 * @file Filter.h
 * @brief The processing unit of the SDK can perform point cloud generation, format conversion and other functions.
 *
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ObTypes.h"

/**
 * @brief Create PointCloud Filter
 *
 * @param[out] error Log error messages
 *
 * @return filter pointcloud_filter object
 *
 */
ob_filter *ob_create_pointcloud_filter(ob_error **error);

/**
 * @brief PointCloud Filter device camera parameters
 *
 * @param[in] filter pointcloud_filter object
 * @param[in] param Camera parameters
 * @param[out] error Log error messages
 *
 */
void ob_pointcloud_filter_set_camera_param(ob_filter *filter, ob_camera_param param, ob_error **error);

/**
 * @brief Set point cloud type parameters
 *
 * @param[in] filter pointcloud_filter object
 * @param[in] type Point cloud type: depth point cloud or RGBD point cloud
 * @param[out] error Log error messages
 *
 */
void ob_pointcloud_filter_set_point_format(ob_filter *filter, ob_format type, ob_error **error);

/**
 * @brief  Set the alignment state of the frames that will be input to produce the point cloud
 * @param[in] filter pointcloud_filter object
 * @param[in] state Alignment status, True: aligned; False: unaligned
 * @param[out] error Log error messages
 *
 */
void ob_pointcloud_filter_set_frame_align_state(ob_filter *filter, bool state, ob_error **error);

/**
 * @brief  Set the point cloud data scaling factor
 * @attention Calling this function to set the scale will change the point coordinate scaling factor of the output point cloud frame: posScale = posScale /
 * scale.The point coordinate scaling factor for the output point cloud frame can be obtained via @ref ob_points_frame_get_position_value_scale function
 * @param[in] filter object
 * @param[in] scale Set the point cloud coordinate data zoom factor
 * @param[out] error Log error messages
 *
 */
void ob_pointcloud_filter_set_position_data_scale(ob_filter *filter, float scale, ob_error **error);

/**
 * @brief  Set point cloud color data normalization
 * @param[in] filter object
 * @param[in] state Sets whether the point cloud color data is normalized
 * @param[out] error Log error messages
 *
 */
void ob_pointcloud_filter_set_color_data_normalization(ob_filter *filter, bool state, ob_error **error);

/**
 * @brief  Set point cloud coordinate system
 * @param[in] filter object
 * @param[in] type coordinate system type
 * @param[out] error Log error messages
 *
 */
void ob_pointcloud_filter_set_coordinate_system(ob_filter *filter, ob_coordinate_system_type type, ob_error **error);

/**
 * @brief Create FormatConvet Filter
 *
 * @param[out] error Log error messages
 *
 * @return filter format_convert object
 *
 */
ob_filter *ob_create_format_convert_filter(ob_error **error);

/**
 * @brief Set the type of format conversion
 *
 * @param[in] filter formatconvet_filter object
 * @param[in] type Format conversion type
 * @param[out] error Log error messages
 *
 */
void ob_format_convert_filter_set_format(ob_filter *filter, ob_convert_format type, ob_error **error);

/**
 * @brief Create compression Filter
 *
 * @param[out] error Log error messages
 *
 * @return filter depth_filter object
 *
 */
ob_filter *ob_create_compression_filter(ob_error **error);

/**
 * @brief Set compression parameters
 *
 * @param[in] filter compression_filter object
 * @param[in] mode Compression mode OB_COMPRESSION_LOSSLESS or OB_COMPRESSION_LOSSY
 * @param[in] params Compression params, struct ob_compression_params, when mode is OB_COMPRESSION_LOSSLESS, params is NULL
 * @param[out] error Log error messages
 *
 */
void ob_compression_filter_set_compression_params(ob_filter *filter, ob_compression_mode mode, void *params, ob_error **error);

/**
 * @brief Create decompression Filter
 *
 * @param[out] error Log error messages
 *
 * @return filter decompression Filter object
 *
 */

ob_filter *ob_create_decompression_filter(ob_error **error);

/**
 * @brief  Filter reset, cache clear, state reset. If the asynchronous interface is used, the processing thread will also be stopped and the pending cache
 * frames will be cleared.
 *
 * @param[in] filter filter object
 * @param[out] error Log error messages
 *
 */
void ob_filter_reset(ob_filter *filter, ob_error **error);

/**
 * @brief Filter processing (synchronous interface)
 *
 * @param[in] filter filter object
 * @param[in] frame pointer to the frame object to be processed
 * @param[out] error Log error messages
 *
 * @return ob_frame  The frame object processed by the filter
 *
 */
ob_frame *ob_filter_process(ob_filter *filter, ob_frame *frame, ob_error **error);

/**
 * @brief Filter Set the processing result callback function (asynchronous callback interface)
 *
 * @param[in] filter filter object
 * @param[in] callback
 * @param[in] user_data Arbitrary user data pointer can be passed in and returned from the callback
 * @param[out] error Log error messages
 *
 */
void ob_filter_set_callback(ob_filter *filter, ob_filter_callback callback, void *user_data, ob_error **error);

/**
 * @brief filter Push the frame into the pending cache (asynchronous callback interface)
 *
 * @param[in] filter filter object
 * @param[out] error Log error messages
 *
 */
void ob_filter_push_frame(ob_filter *filter, ob_frame *frame, ob_error **error);

/**
 * @brief Delete Filter
 *
 * @param[in] filter filter object
 * @param[out] error Log error messages
 *
 */
void ob_delete_filter(ob_filter *filter, ob_error **error);

#ifdef __cplusplus
}
#endif
