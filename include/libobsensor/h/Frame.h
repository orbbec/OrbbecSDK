/**
 * @file Frame.h
 * @brief Frame related function is mainly used to obtain frame data and frame information
 *
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ObTypes.h"

/**
 * @brief Get the frame index
 *
 * @param[in] frame Frame object
 * @param[out] error Log wrong message
 * @return uint64_t returns the frame index
 *
 */
uint64_t ob_frame_index(ob_frame *frame, ob_error **error);

/**
 * @brief Get the frame format
 *
 * @param[in] frame Frame object
 * @param[out] error  Log error messages
 * @return ob_format returns the frame format
 *
 */
ob_format ob_frame_format(ob_frame *frame, ob_error **error);

/**
 * @brief Get the frame type
 *
 * @param[in] frame Frame object
 * @param[out] error Log error messages
 * @return ob_frame_type returns the frame type
 *
 */
ob_frame_type ob_frame_get_type(ob_frame *frame, ob_error **error);

/**
 * @brief Get frame time stamp (hardware)
 *
 * @param[in] frame Frame object
 * @param[out] error Log error messages
 * @return uint64_t returns the frame hardware timestamp
 *
 */
uint64_t ob_frame_time_stamp(ob_frame *frame, ob_error **error);

/**
 * @brief Get frame time stamp (hardware) us
 *
 * @param[in] frame Frame object
 * @param[out] error Log error messages
 * @return uint64_t returns the frame hardware timestamp, unit us
 *
 */
uint64_t ob_frame_time_stamp_us(ob_frame *frame, ob_error **error);

/**
 * @brief Get frame time stamp (system)
 *
 * @param[in] frame Frame object
 * @param[out] error Log error messages
 * @return uint64_t returns the frame system timestamp
 *
 */
uint64_t ob_frame_system_time_stamp(ob_frame *frame, ob_error **error);

/**
 * @brief Get frame data
 *
 * @param[in] frame Frame object
 * @param[out] error Log error messages
 * @return void* * returns frame data pointer
 *
 */
void *ob_frame_data(ob_frame *frame, ob_error **error);

/**
 * @brief Get the frame data size
 *
 * @param[in] frame Frame object
 * @param[out] error Log error messages
 * @return uint32_t returns the frame data size
 * If it is point cloud data, it returns the number of bytes occupied by all point sets. If you need to find the number of points, you need to divide dataSize
 * by the structure size of the corresponding point type.
 */
uint32_t ob_frame_data_size(ob_frame *frame, ob_error **error);

/**
 * @brief Get video frame width
 *
 * @param[in] frame Frame object
 * @param[out] error Log error messages
 * @return uint32_t returns the frame width
 *
 */
uint32_t ob_video_frame_width(ob_frame *frame, ob_error **error);

/**
 * @brief Get video frame height
 *
 * @param[in] frame  Frame object
 * @param[out] error Log error messages
 * @return uint32_t returns the frame height
 *
 */
uint32_t ob_video_frame_height(ob_frame *frame, ob_error **error);

/**
 * @brief Get the metadata of the frame
 *
 * @param[in] frame  Video frame object
 * @param[out] error Log error messages
 * @return void* returns the metadata pointer of the frame
 *
 */
void *ob_video_frame_metadata(ob_frame *frame, ob_error **error);

/**
 * @brief Get the metadata size of the frame
 *
 * @param[in] frame Video frame object
 * @param[out] error Log error messages
 * @return uint32_t returns the metadata size of the frame
 *
 */
uint32_t ob_video_frame_metadata_size(ob_frame *frame, ob_error **error);

/**
 * @brief  Get the effective number of pixels (such as Y16 format frame, but only the lower 10 bits are effective bits, and the upper 6 bits are filled with 0)
 * @attention Only valid for Y8/Y10/Y11/Y12/Y14/Y16 format
 *
 * @param[in] frame video frame object
 * @param[out] error log error messages
 * @return uint8_t return the effective number of pixels in the pixel, or 0 if it is an unsupported format
 *
 */
uint8_t ob_video_frame_pixel_available_bit_size(ob_frame *frame, ob_error **error);

/* @brief 查询IR frame的最原始数据来源（即使回放视频，其原始的数据源也是sensor）
 * @param[in] frame 视频帧对象
 * @param[out] error 记录错误信息
 * @return uint8_t 返回
 */
ob_sensor_type ob_ir_frame_get_source_sensor_type(ob_frame *frame, ob_error **ob_error);

/**
 * @brief Get the value scale of the depth frame. The pixel value of depth frame is multiplied by the scale to give a depth value in millimeter.
 *      such as valueScale=0.1, and a certain coordinate pixel value is pixelValue=10000, then the depth value = pixelValue*valueScale = 10000*0.1=1000mm。
 *
 * @param[in] frame Frame object
 * @param[out] error Log error messages
 * @return float value scale
 *
 */
float ob_depth_frame_get_value_scale(ob_frame *frame, ob_error **error);

/**
 * @brief Get the point position value scale of the points frame. the point position value of points frame is multiplied by the scale to give a position value
 * in millimeter. such as scale=0.1, The x-coordinate value of a point is x = 10000, which means that the actual x-coordinate value = x*scale = 10000*0.1 =
 * 1000mm.
 *
 * @param[in] frame Frame object
 * @param[out] error Log error messages
 * @return float position value scale
 *
 */
float ob_points_frame_get_position_value_scale(ob_frame *frame, ob_error **error);

/**
 * @brief Delete frame
 *
 * @param[in] frame  The frame object to delete
 * @param[out] error Log error messages
 *
 */
void ob_delete_frame(ob_frame *frame, ob_error **error);

/**
 * @brief Get the number of frames contained in the frameset
 *
 * @param[in] frameset frameset object
 * @param[out] error Log error messages
 * @return uint32_t returns the number of frames
 *
 */
uint32_t ob_frameset_frame_count(ob_frame *frameset, ob_error **error);

/**
 * @brief Get the depth frame from the frameset
 *
 * @param[in] frameset  Frameset object
 * @param[out] error Log error messages
 * @return ob_frame* returns the depth frame
 *
 */
ob_frame *ob_frameset_depth_frame(ob_frame *frameset, ob_error **error);

/**
 * @brief Get the color frame from the frameset
 *
 * @param[in] frameset Frameset object
 * @param[out] error Log error messages
 * @return ob_frame* returns the color frame
 *
 */
ob_frame *ob_frameset_color_frame(ob_frame *frameset, ob_error **error);

/**
 * @brief Get the infrared frame from the frameset
 *
 * @param[in] frameset Frameset object
 * @param[out] error Log error messages
 * @return ob_frame* returns the infrared frame
 *
 */
ob_frame *ob_frameset_ir_frame(ob_frame *frameset, ob_error **error);

/**
 * @brief Get point cloud data from the frameset
 *
 * @param[in] frameset Frameset object
 * @param[out] error Log error messages
 * @return ob_frame* returns the point cloud frame
 *
 */
ob_frame *ob_frameset_points_frame(ob_frame *frameset, ob_error **error);

/**
 * @brief Get frame frame frameSetByte type.
 *
 * @param[in] frameset Frameset object
 * @param[in] frame_type frame type
 * @param[out] error Log error messages
 * @return ob_frame* if frame_type exits, or nullptr.
 *
 */
ob_frame *ob_frameset_get_frame(ob_frame *frameset, ob_frame_type frame_type, ob_error **error);

/**
 * @brief Get accelerometer frame data
 *
 * @param[in] frame Accelerometer frame
 * @param[out] error  Log error messages
 * @return ob_accel_value accelerometer data
 *
 */
ob_accel_value ob_accel_frame_value(ob_frame *frame, ob_error **error);

/**
 * @brief Get the temperature when acquiring the accelerometer frame
 *
 * @param[in] frame Accelerometer frame
 * @param[out] error Log error messages
 * @return float return value
 *
 */
float ob_accel_frame_temperature(ob_frame *frame, ob_error **error);

/**
 * @brief Get gyroscope frame data
 *
 * @param[in] frame Gyro Frame
 * @param[out] error Log error messages
 * @return ob_gyro_value gyroscope data
 *
 */
ob_gyro_value ob_gyro_frame_value(ob_frame *frame, ob_error **error);

/**
 * @brief Get the temperature when acquiring the gyroscope frame
 *
 * @param[in] frame Accelerometer frame
 * @param[out] error Log error messages
 * @return float return value
 *
 */
float ob_gyro_frame_temperature(ob_frame *frame, ob_error **error);

/**
 * @brief Increase the reference count
 *
 * @param[in] frame the frame object to increase the reference count
 * @param[out] error Log error messages
 *
 */
void ob_frame_add_ref(ob_frame *frame, ob_error **error);

/**
 * @brief Create a frame object based on an externally created Buffer
 *
 * @param[in] frame_format frame object format
 * @param[in] width frame object width
 * @param[in] height frame object height
 * @param[in] stride_bytes buffer row span
 * @param[in] frame_type frame object type
 * @param[out] error Log error messages
 * @return std::shared_ptr<Frame> return empty frame object
 *
 */
ob_frame *ob_create_frame(ob_format frame_format, int width, int height, int stride_bytes, ob_frame_type frame_type, ob_error **error);

/**
 * @brief Create a frame object based on an externally created Buffer
 *
 * @param[in] frame_format frame object format
 * @param[in] frame_width frame object width
 * @param[in] frame_height frame object height
 * @param[in] buffer frame object buffer
 * @param[in] buffer_size frame object buffer size
 * @param[in] buffer_destroy_cb destroy callback
 * @param[in] buffer_destroy_context destroy context
 * @param[out] error Log error messages
 * @return std::shared_ptr<Frame> return frame object
 *
 */
ob_frame *ob_create_frame_from_buffer(ob_format frame_format, uint32_t frame_width, uint32_t frame_height, uint8_t *buffer, uint32_t buffer_size,
                                      ob_frame_destroy_callback *buffer_destroy_cb, void *buffer_destroy_context, ob_error **error);

/**
 * @brief Creates an empty frame collection object
 *
 * @param[out] error Log error messages
 * @return std::shared_ptr<Frame> return frameset object
 *
 */
ob_frame *ob_create_frameset(ob_error **error);

/**
 * @brief Populate the frame collection with frames of the corresponding type
 *
 * @param[in] frameset frameset object
 * @param[in] type the type of frame filled in
 * @param[in] frame the object that fills the frame
 * @param[out] error Log error messages
 *
 */
void ob_frameset_push_frame(ob_frame *frameset, ob_frame_type type, ob_frame *frame, ob_error **error);

/**
 * @brief Set the system timestamp of the frame
 *
 * @param[in] frame object for frame settings
 * @param[in] systemTimestamp set by systemTimestamp
 * @param[out] error Log error messages
 *
 */
void ob_frame_set_system_time_stamp(ob_frame *frame, uint64_t system_timestamp, ob_error **error);

/**
 * @brief Set the device timestamp of the frame
 *
 * @param[in] frame object for frame settings
 * @param[in] device_timestamp set by device_timestamp
 * @param[out] error Log error messages
 *
 */
void ob_frame_set_device_time_stamp(ob_frame *frame, uint64_t device_timestamp, ob_error **error);

/**
 * @brief Set the device timestamp of the frame
 *
 * @param[in] frame object for frame settings
 * @param[in] device_timestamp_us the device timestamp set (Us)
 * @param[out] error Log error messages
 *
 */
void ob_frame_set_device_time_stamp_us(ob_frame *frame, uint64_t device_timestamp_us, ob_error **error);

#ifdef __cplusplus
}
#endif
