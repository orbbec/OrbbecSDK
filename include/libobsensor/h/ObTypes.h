// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Orbbec  Corporation. All Rights Reserved.

/**
 * @file ObTypes.h
 * @brief Provide structs commonly used in the SDK, enumerating constant definitions.
 *
 */

#pragma once

#if(defined(WIN32) || defined(_WIN32) || defined(WINCE))
#ifdef OB_EXPORTS
#define OB_EXTENSION_API __declspec(dllexport)
#define OB_EXTENSION_INTERNAL_API __declspec(dllexport)
#else
#define OB_EXTENSION_API __declspec(dllimport)
#define OB_EXTENSION_INTERNAL_API __declspec(dllimport)
#endif
#else
#define OB_EXTENSION_API __attribute__((visibility("default")))
#define OB_EXTENSION_INTERNAL_API __attribute__((visibility("default")))
#endif

#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

#pragma pack(push, 1)  // struct 1-byte align

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct ContextImpl             ob_context;
typedef struct DeviceImpl              ob_device;
typedef struct DeviceInfoImpl          ob_device_info;
typedef struct DeviceListImpl          ob_device_list;
typedef struct CameraParamListImpl     ob_camera_param_list;
typedef struct SensorImpl              ob_sensor;
typedef struct SensorListImpl          ob_sensor_list;
typedef struct StreamProfileImpl       ob_stream_profile;
typedef struct StreamProfileListImpl   ob_stream_profile_list;
typedef struct CFrameImpl              ob_frame;
typedef struct FilterImpl              ob_filter;
typedef struct PipelineImpl            ob_pipeline;
typedef struct ConfigImpl              ob_config;
typedef struct RecorderImpl            ob_recorder;
typedef struct PlaybackImpl            ob_playback;
typedef struct OBDepthWorkModeListImpl ob_depth_work_mode_list;

/**
 * @brief the permission type of api or property
 */
typedef enum {
    OB_PERMISSION_DENY       = 0,  // no permission
    OB_PERMISSION_READ       = 1,  // can read
    OB_PERMISSION_WRITE      = 2,  // can write
    OB_PERMISSION_READ_WRITE = 3,  // can read and write
} OBPermissionType,
    ob_permission_type;

/**
 * @brief error code
 */
typedef enum {
    OB_STATUS_OK    = 0,  // status ok
    OB_STATUS_ERROR = 1,  // status error
} OBStatus,
    ob_status;

/**
 * @brief log level, the higher the level, the stronger the log filter
 *
 */
typedef enum {
    OB_LOG_SEVERITY_DEBUG,  // debug
    OB_LOG_SEVERITY_INFO,   // information
    OB_LOG_SEVERITY_WARN,   // warning
    OB_LOG_SEVERITY_ERROR,  // error
    OB_LOG_SEVERITY_FATAL,  // fatal error
    OB_LOG_SEVERITY_NONE    // none (close LOG)
} OBLogSeverity,
    ob_log_severity, DEVICE_LOG_SEVERITY_LEVEL, OBDeviceLogSeverityLevel, ob_device_log_severity_level;

/**
 * @brief The exception types in the SDK, through the exception type, you can easily determine the specific type of error.
 * For detailed error API interface functions and error logs, please refer to the information of ob_error
 *
 */
typedef enum {
    OB_EXCEPTION_TYPE_UNKNOWN,              /// Unknown error, an error not clearly defined by the SDK
    OB_EXCEPTION_TYPE_CAMERA_DISCONNECTED,  /// SDK device disconnection exception
    OB_EXCEPTION_TYPE_PLATFORM,             /// An error in the SDK adaptation platform layer means an error in the implementation of a specific system platform
    OB_EXCEPTION_TYPE_INVALID_VALUE,        /// Invalid parameter type exception, need to check input parameter
    OB_EXCEPTION_TYPE_WRONG_API_CALL_SEQUENCE,  // Exception caused by API version mismatch
    OB_EXCEPTION_TYPE_NOT_IMPLEMENTED,          /// SDK and firmware have not yet implemented functions
    OB_EXCEPTION_TYPE_IO,                       /// SDK access IO exception error
    OB_EXCEPTION_TYPE_MEMORY,                   /// SDK access and use memory errors, which means that the frame fails to allocate memory
    OB_EXCEPTION_TYPE_UNSUPPORTED_OPERATION,    /// Unsupported operation type error by SDK or RGBD device
} OBExceptionType,
    ob_exception_type;

/**
 * @brief The error class exposed by the SDK, users can get detailed error information according to the erro
 *
 */
typedef struct ob_error {
    ob_status         status;          // Describe the status code of the error, as compatible with previous customer status code requirements
    char              message[256];    // Describe the detailed error log
    char              function[256];   // Describe the name of the function where the error occurred
    char              args[256];       // Describes the parameters passed to the function when an error occurs. Used to check whether the parameter is wrong
    ob_exception_type exception_type;  // The description is the specific error type of the SDK
} ob_error;

/**
 * @brief Enumeration value describing the sensor type
 *
 */
typedef enum {
    OB_SENSOR_UNKNOWN  = 0,  // Unknown type sensor
    OB_SENSOR_IR       = 1,  // IR
    OB_SENSOR_COLOR    = 2,  // Color
    OB_SENSOR_DEPTH    = 3,  // Depth
    OB_SENSOR_ACCEL    = 4,  // Accel
    OB_SENSOR_GYRO     = 5,  // Gyro
    OB_SENSOR_IR_LEFT  = 6,  // left IR
    OB_SENSOR_IR_RIGHT = 7,  // Right IR
} OBSensorType,
    ob_sensor_type;

/**
 * @brief Enumeration value describing the type of data stream
 *
 */
typedef enum {
    OB_STREAM_UNKNOWN  = -1,  // Unknown type stream
    OB_STREAM_VIDEO    = 0,   // Video stream (infrared, color, depth streams are all video streams)
    OB_STREAM_IR       = 1,   // IR stream
    OB_STREAM_COLOR    = 2,   // color stream
    OB_STREAM_DEPTH    = 3,   // depth stream
    OB_STREAM_ACCEL    = 4,   // Accelerometer data stream
    OB_STREAM_GYRO     = 5,   // Gyroscope data stream
    OB_STREAM_IR_LEFT  = 6,   // Left IR stream
    OB_STREAM_IR_RIGHT = 7,   // Right IR stream
} OBStreamType,
    ob_stream_type;

/**
 * @brief Describe the Frame type enumeration value
 *
 */
typedef enum {
    OB_FRAME_UNKNOWN  = -1,  // Unknown type frame
    OB_FRAME_VIDEO    = 0,   // Describes the Frame type enumeration value
    OB_FRAME_IR       = 1,   // IR frame
    OB_FRAME_COLOR    = 2,   // color frame
    OB_FRAME_DEPTH    = 3,   // depth frame
    OB_FRAME_ACCEL    = 4,   // Accelerometer data frame
    OB_FRAME_SET      = 5,   // Frame collection (internally contains a variety of data frames)
    OB_FRAME_POINTS   = 6,   // point cloud frame
    OB_FRAME_GYRO     = 7,   // Gyroscope data frame
    OB_FRAME_IR_LEFT  = 8,   // Left IR frame
    OB_FRAME_IR_RIGHT = 9,   // Right IR frame
} OBFrameType,
    ob_frame_type;

/**
 * @brief Enumeration value describing the pixel format
 *
 */
typedef enum {
    OB_FORMAT_YUYV       = 0,     //< \if English YUYV format
    OB_FORMAT_YUY2       = 1,     //< \if English YUY2 format (the actual format is the same as YUYV)
    OB_FORMAT_UYVY       = 2,     //< \if English UYVY format
    OB_FORMAT_NV12       = 3,     //< \if English NV12 format
    OB_FORMAT_NV21       = 4,     //< \if English NV21 format
    OB_FORMAT_MJPG       = 5,     //< \if English MJPEG encoding format
    OB_FORMAT_H264       = 6,     //< \if English H.264 encoding format
    OB_FORMAT_H265       = 7,     //< \if English H.265 encoding format
    OB_FORMAT_Y16        = 8,     //< \if English Y16 format, single channel 16bit depth
    OB_FORMAT_Y8         = 9,     //< \if English Y8 format, single channel 8bit depth
    OB_FORMAT_Y10        = 10,    //<  Y10 format, single channel 10bit depth (SDK will unpack into Y16 by default)
    OB_FORMAT_Y11        = 11,    /// Y11 format, single channel 11bit depth (SDK will unpack into Y16 by default)
    OB_FORMAT_Y12        = 12,    /// Y12 format, single channel 12bit depth (SDK will unpack into Y16 by default)
    OB_FORMAT_GRAY       = 13,    // GRAY (the actual format is the same as YUYV)
    OB_FORMAT_HEVC       = 14,    // HEVC encoding format (the actual format is the same as H265)
    OB_FORMAT_I420       = 15,    // I420 format
    OB_FORMAT_ACCEL      = 16,    // Acceleration data format
    OB_FORMAT_GYRO       = 17,    // Gyroscope Data Format
    OB_FORMAT_POINT      = 19,    // xyz 3D coordinate point format
    OB_FORMAT_RGB_POINT  = 20,    // xyz 3D coordinate point format with RGB information
    OB_FORMAT_RLE        = 21,    // RLE pressure test format (SDK will be unpacked into Y16 by default)
    OB_FORMAT_RGB        = 22,    // RGB format (actual BRG888)
    OB_FORMAT_BGR        = 23,    // BGR format (actual BRG888)
    OB_FORMAT_Y14        = 24,    /// Y14 format, single channel 14bit depth (SDK will unpack into Y16 by default)
    OB_FORMAT_BGRA       = 25,    // BGRA format
    OB_FORMAT_COMPRESSED = 26,    // Compression format
    OB_FORMAT_UNKNOWN    = 0xff,  // unknown format
} OBFormat,
    ob_format;

#define OB_FORMAT_RGB888 OB_FORMAT_RGB  // Alias of OB_FORMAT_RGB for compatible
#define OB_FORMAT_MJPEG OB_FORMAT_MJPG  // Alias of OB_FORMAT_MJPG for compatible

/**
 * @brief Firmware upgrade status
 */
typedef enum {
    STAT_FILE_TRANSFER = 4,   // file transfer
    STAT_DONE          = 3,   // update completed
    STAT_IN_PROGRESS   = 2,   // upgrade in process
    STAT_START         = 1,   // start the upgrade
    STAT_VERIFY_IMAGE  = 0,   // Image file verification
    ERR_VERIFY         = -1,  // Verification failed
    ERR_PROGRAM        = -2,  // Program execution failed
    ERR_ERASE          = -3,  // Flash parameter failed
    ERR_FLASH_TYPE     = -4,  // Flash type error
    ERR_IMAGE_SIZE     = -5,  // Image file size error
    ERR_OTHER          = -6,  // other errors
    ERR_DDR            = -7,  // DDR access error
    ERR_TIMEOUT        = -8   // timeout error
} OBUpgradeState,
    ob_upgrade_state;
/**
 * @brief file transfer status
 *
 */
typedef enum {
    FILE_TRAN_STAT_TRANSFER         = 2,   // file transfer
    FILE_TRAN_STAT_DONE             = 1,   // file transfer succeeded
    FILE_TRAN_STAT_PREPAR           = 0,   // preparing
    FILE_TRAN_ERR_DDR               = -1,  // DDR access failed
    FILE_TRAN_ERR_NOT_ENOUGH_SPACE  = -2,  // Insufficient target space error
    FILE_TRAN_ERR_PATH_NOT_WRITABLE = -3,  // Destination path is not writable
    FILE_TRAN_ERR_MD5_ERROR         = -4,  // MD5 checksum error
    FILE_TRAN_ERR_WRITE_FLASH_ERROR = -5,  // write flash error
    FILE_TRAN_ERR_TIMEOUT           = -6   // timeout error
} OBFileTranState,
    ob_file_tran_state;

/**
 * @brief data transfer status
 *
 */
typedef enum {
    DATA_TRAN_STAT_STOPPED      = 3,   // data transfer stoped
    DATA_TRAN_STAT_DONE         = 2,   // data transfer completed
    DATA_TRAN_STAT_VERIFYING    = 1,   // data verifying
    DATA_TRAN_STAT_TRANSFERRING = 0,   // data transferring
    DATA_TRAN_ERR_BUSY          = -1,  // Transmission is busy
    DATA_TRAN_ERR_UNSUPPORTED   = -2,  // not support
    DATA_TRAN_ERR_TRAN_FAILED   = -3,  // transfer failed
    DATA_TRAN_ERR_VERIFY_FAILED = -4,  // Test failed
    DATA_TRAN_ERR_OTHER         = -5   // other errors
} OBDataTranState,
    ob_data_tran_state;

/**
 * @brief Data block structure for data block transmission
 */
typedef struct {
    void *   data;          // current block data pointer
    uint32_t size;          // Current block data length
    uint32_t offset;        // The offset of the current data block relative to the complete data
    uint32_t fullDataSize;  // full data size
} OBDataChunk, ob_data_chunk;

/**
 * @brief Int range structure
 */
typedef struct {
    int32_t cur;   // current value
    int32_t max;   // maximum value
    int32_t min;   // minimum value
    int32_t step;  // step value
    int32_t def;   // Default
} OBIntPropertyRange, ob_int_property_range;

/**
 * @brief Float range structure
 */
typedef struct {
    float cur;   // current value
    float max;   // maximum value
    float min;   // minimum value
    float step;  // step value
    float def;   // default
} OBFloatPropertyRange, ob_float_property_range;

/**
 * @brief Boolean-scoped structure
 */
typedef struct {
    bool cur;   // current value
    bool max;   // maximum value
    bool min;   // minimum value
    bool step;  // step value
    bool def;   // default
} OBBoolPropertyRange, ob_bool_property_range;

/**
 * @brief Camera internal parameters
 */
typedef struct {
    float   fx;      // focal length in x direction
    float   fy;      // focal length in y direction
    float   cx;      // Optical center abscissa
    float   cy;      // Optical center ordinate
    int16_t width;   // image width
    int16_t height;  // image height
} OBCameraIntrinsic, ob_camera_intrinsic;

/**
 * @brief Distortion Parameters
 */
typedef struct {
    float k1;  // Radial distortion factor 1
    float k2;  // Radial distortion factor 2
    float k3;  // Radial distortion factor 3
    float k4;  // Radial distortion factor 4
    float k5;  // Radial distortion factor 5
    float k6;  // Radial distortion factor 6
    float p1;  // Tangential distortion factor 1
    float p2;  // Tangential distortion factor 2
} OBCameraDistortion, ob_camera_distortion;

/**
 * @brief Rotation/Transformation
 */
typedef struct {
    float rot[9];    // Rotation matrix
    float trans[3];  // transformation matrix
} OBD2CTransform, ob_d2c_transform;

/**
 * @brief Camera parameters
 */
typedef struct {
    OBCameraIntrinsic  depthIntrinsic;   // Depth camera internal parameters
    OBCameraIntrinsic  rgbIntrinsic;     // Color camera internal parameters
    OBCameraDistortion depthDistortion;  // Depth camera distortion parameters
    OBCameraDistortion rgbDistortion;    // Color camera distortion parameters 1
    OBD2CTransform     transform;        // rotation/transformation matrix

    // TODO:
    bool isMirrored;  // Whether the image frame corresponding to this group of parameters is mirrored
} OBCameraParam, ob_camera_param;

/**
 * @brief Camera parameters
 */
typedef struct {
    OBCameraIntrinsic  depthIntrinsic;   // Depth camera internal parameters
    OBCameraIntrinsic  rgbIntrinsic;     // Color camera internal parameters
    OBCameraDistortion depthDistortion;  // Depth camera distortion parameters
    OBCameraDistortion rgbDistortion;    // Color camera distortion parameters 1
    OBD2CTransform     transform;        // rotation/transformation matrix
} OBCameraParam_V0, ob_camera_param_v0;

/**
 * @brief depth margin filter config
 */
typedef struct MarginFilterConfig_S {
    int      margin_x_th;       // Horizontal threshold settings
    int      margin_y_th;       // Vertical threshold settings
    int      limit_x_th;        // The maximum horizontal threshold
    int      limit_y_th;        // The maximum Vertical threshold
    uint32_t width;             // image width
    uint32_t height;            // image height
    bool     enable_direction;  // Set true to horizontal and vertical, set false to horizontal only
} ob_margin_filter_config, OBMarginFilterConfig;

/**
 * @brief alignment mode
 */
typedef enum {
    ALIGN_DISABLE,      // turn off alignment
    ALIGN_D2C_HW_MODE,  // Hardware D2C alignment mode
    ALIGN_D2C_SW_MODE,  // Software D2C alignment mode
} OBAlignMode,
    ob_align_mode;

/**
 * @brief rectangle
 */
typedef struct {
    uint32_t x;       // origin coordinate x
    uint32_t y;       // origin coordinate y
    uint32_t width;   // rectangle width
    uint32_t height;  // rectangle height
} OBRect, ob_rect;

/**
 * @brief format conversion type enumeration
 */
typedef enum {
    FORMAT_YUYV_TO_RGB888 = 0,  // YUYV to RGB888
    FORMAT_I420_TO_RGB888,      // I420 to RGB888
    FORMAT_NV21_TO_RGB888,      // NV21 to RGB888
    FORMAT_NV12_TO_RGB888,      // NV12 to RGB888
    FORMAT_MJPG_TO_I420,        // MJPG to I420
    FORMAT_RGB888_TO_BGR,       // RGB888 to BGR
    FORMAT_MJPG_TO_NV21,        // MJPG to NV21
    FORMAT_MJPG_TO_RGB888,      // MJPG to RGB888
    FORMAT_MJPG_TO_BGR888,      // MJPG to BGR888
    FORMAT_MJPG_TO_BGRA,        // MJPG to BGRA
    FORMAT_UYVY_TO_RGB888,      // UYVY to RGB888
    FORMAT_BGR_TO_RGB,          // BGR to RGB
} OBConvertFormat,
    ob_convert_format;

// DEPRECATED: 仅用于旧版本程序兼容，会在后续迭代版本彻底删除
#define FORMAT_MJPEG_TO_I420 FORMAT_MJPG_TO_I420
#define FORMAT_MJPEG_TO_NV21 FORMAT_MJPG_TO_NV21
#define FORMAT_MJPEG_TO_RGB888 FORMAT_MJPG_TO_RGB888
#define FORMAT_MJPEG_TO_BGR888 FORMAT_MJPG_TO_BGR888
#define FORMAT_MJPEG_TO_BGRA FORMAT_MJPG_TO_BGRA

/**
 * @brief Enumeration of IMU sample rate values ​​(gyroscope or accelerometer)
 */
typedef enum {
    OB_SAMPLE_RATE_1_5625_HZ = 1,  // 1.5625Hz */
    OB_SAMPLE_RATE_3_125_HZ,       // 3.125Hz */
    OB_SAMPLE_RATE_6_25_HZ,        // 6.25Hz */
    OB_SAMPLE_RATE_12_5_HZ,        // 12.5Hz */
    OB_SAMPLE_RATE_25_HZ,          // 25Hz */
    OB_SAMPLE_RATE_50_HZ,          // 50Hz */
    OB_SAMPLE_RATE_100_HZ,         // 100Hz */
    OB_SAMPLE_RATE_200_HZ,         // 200Hz */
    OB_SAMPLE_RATE_500_HZ,         // 500Hz */
    OB_SAMPLE_RATE_1_KHZ,          // 1KHz */
    OB_SAMPLE_RATE_2_KHZ,          // 2KHz */
    OB_SAMPLE_RATE_4_KHZ,          // 4KHz */
    OB_SAMPLE_RATE_8_KHZ,          // 8KHz */
    OB_SAMPLE_RATE_16_KHZ,         // 16KHz */
    OB_SAMPLE_RATE_32_KHZ,         // 32Hz */
} OBGyroSampleRate,
    ob_gyro_sample_rate, OBAccelSampleRate, ob_accel_sample_rate, OB_SAMPLE_RATE;

/**
 * @brief Enumeration of gyroscope ranges
 */
typedef enum {
    OB_GYRO_FS_16dps = 1,  // 16 degrees per second
    OB_GYRO_FS_31dps,      // 31 degrees per second
    OB_GYRO_FS_62dps,      // 62 degrees per second
    OB_GYRO_FS_125dps,     // 125 degrees per second
    OB_GYRO_FS_250dps,     // 250 degrees per second
    OB_GYRO_FS_500dps,     // 500 degrees per second
    OB_GYRO_FS_1000dps,    // 1000 degrees per second
    OB_GYRO_FS_2000dps,    // 2000 degrees per second
} OBGyroFullScaleRange,
    ob_gyro_full_scale_range, OB_GYRO_FULL_SCALE_RANGE;

/**
 * @brief Accelerometer range enumeration
 */
typedef enum {
    OB_ACCEL_FS_2g = 1,  // 1x the acceleration of gravity
    OB_ACCEL_FS_4g,      // 4x the acceleration of gravity
    OB_ACCEL_FS_8g,      // 8x the acceleration of gravity
    OB_ACCEL_FS_16g,     // 16x the acceleration of gravity
} OBAccelFullScaleRange,
    ob_accel_full_scale_range, OB_ACCEL_FULL_SCALE_RANGE;

/**
 * @brief Data structures for accelerometers and gyroscopes
 */
typedef struct {
    float x;  // x-direction component
    float y;  // y-direction component
    float z;  // z-direction component
} OBAccelValue, OBGyroValue, ob_accel_value, ob_gyro_value;

/**
 * @brief Device state
 */
typedef uint64_t OBDeviceState, ob_device_state;

/**
 * @brief Get the temperature parameters of the device (unit: Celsius)
 */
typedef struct {
    float cpuTemp;         // CPU temperature
    float irTemp;          // IR temperature
    float ldmTemp;         // laser temperature
    float mainBoardTemp;   // motherboard temperature
    float tecTemp;         // TEC temperature
    float imuTemp;         // IMU temperature
    float rgbTemp;         // RGB temperature
    float irLeftTemp;      // Left IR temperature
    float irRightTemp;     // Right IR temperature
    float chipTopTemp;     // MX6600 top temperature
    float chipBottomTemp;  // MX6600 bottom temperature
} OBDeviceTemperature, ob_device_temperature, DEVICE_TEMPERATURE;

/**
 * @brief Depth crop mode enumeration
 */
typedef enum {
    DEPTH_CROPPING_MODE_AUTO  = 0,  // automatic mode
    DEPTH_CROPPING_MODE_CLOSE = 1,  // close crop
    DEPTH_CROPPING_MODE_OPEN  = 2,  // open crop
} OBDepthCroppingMode,
    ob_depth_cropping_mode, OB_DEPTH_CROPPING_MODE;

/**
 * @brief device type enumeration
 */
typedef enum {
    OB_STRUCTURED_LIGHT_MONOCULAR_CAMERA = 0,  // Monocular structured light camera
    OB_STRUCTURED_LIGHT_BINOCULAR_CAMERA = 1,  // Binocular structured light camera
    OB_TOF_CAMERA                        = 2,  // TOF camera
} OBDeviceType,
    ob_device_type, OB_DEVICE_TYPE;

/**
 * @brief record playback of the type of interest
 */
typedef enum {
    OB_MEDIA_COLOR_STREAM    = 1,    // color stream
    OB_MEDIA_DEPTH_STREAM    = 2,    // depth stream
    OB_MEDIA_IR_STREAM       = 4,    // IR stream
    OB_MEDIA_GYRO_STREAM     = 8,    // gyro stream
    OB_MEDIA_ACCEL_STREAM    = 16,   // accel stream
    OB_MEDIA_CAMERA_PARAM    = 32,   // camera parameter
    OB_MEDIA_DEVICE_INFO     = 64,   // device information
    OB_MEDIA_STREAM_INFO     = 128,  // stream information
    OB_MEDIA_IR_LEFT_STREAM  = 256,  // Left IR stream
    OB_MEDIA_IR_RIGHT_STREAM = 512,  // Right Left IR stream

    OB_MEDIA_ALL = OB_MEDIA_COLOR_STREAM | OB_MEDIA_DEPTH_STREAM | OB_MEDIA_IR_STREAM | OB_MEDIA_GYRO_STREAM | OB_MEDIA_ACCEL_STREAM | OB_MEDIA_CAMERA_PARAM
                   | OB_MEDIA_DEVICE_INFO | OB_MEDIA_STREAM_INFO | OB_MEDIA_IR_LEFT_STREAM | OB_MEDIA_IR_RIGHT_STREAM,  // All media data types
} OBMediaType,
    ob_media_type, OB_MEDIA_TYPE;

/**
 * @brief Record playback status
 */
typedef enum {
    OB_MEDIA_BEGIN = 0,  // begin
    OB_MEDIA_PAUSE,      // pause
    OB_MEDIA_RESUME,     // resume
    OB_MEDIA_END,        // end
} OBMediaState,
    ob_media_state, OB_MEDIA_STATE_EM;

/**
 * @brief depth accuracy class
 * @attention The depth accuracy level does not completely determine the depth unit and real accuracy, and the influence of the data packaging format needs to
 * be considered. The specific unit can be obtained through getValueScale() of DepthFrame
 */
typedef enum {
    OB_PRECISION_1MM,   // 1mm */
    OB_PRECISION_0MM8,  // 0.8mm */
    OB_PRECISION_0MM4,  // 0.4mm */
    OB_PRECISION_0MM1,  // 0.1mm */
    OB_PRECISION_0MM2,  // 0.2mm */
    OB_PRECISION_COUNT,
} OBDepthPrecisionLevel,
    ob_depth_precision_level, OB_DEPTH_PRECISION_LEVEL;

/**
 * @brief tof filter scene range
 *
 */
typedef enum {
    OB_TOF_FILTER_RANGE_CLOSE  = 0,    // close range
    OB_TOF_FILTER_RANGE_MIDDLE = 1,    // middle range
    OB_TOF_FILTER_RANGE_LONG   = 2,    // long range
    OB_TOF_FILTER_RANGE_DEBUG  = 100,  // debug range
} OBTofFilterRange,
    ob_tof_filter_range, TOF_FILTER_RANGE;

/**
 * @brief 3D point structure in SDK
 *
 */
typedef struct {
    float x;  // x coordinate
    float y;  // y coordinate
    float z;  // z coordinate
} OBPoint, ob_point;

/**
 * @brief 3D point structure with color information
 *
 */
typedef struct {
    float x;  // x coordinate
    float y;  // y coordinate
    float z;  // z coordinate
    float r;  // red channel component
    float g;  // green channel component
    float b;  // blue channel component
} OBColorPoint, ob_color_point;

typedef enum {
    OB_COMPRESSION_LOSSLESS = 0,
    OB_COMPRESSION_LOSSY    = 1,
} OBCompressionMode,
    ob_compression_mode, OB_COMPRESSION_MODE;

/**
 * Compression Params
 *
 */
typedef struct {
    /*
     * Lossy compression threshold, range [0~255], recommended value is 9, the higher the threshold, the higher the compression ratio.
     *
     */
    int threshold;
} OBCompressionParams, ob_compression_params, OB_COMPRESSION_PARAMS;

/**
 * @brief TOF Exposure Threshold
 */
typedef struct {
    int32_t upper;  // Upper threshold, unit: ms
    int32_t lower;  // Lower threshold, unit: ms
} OBTofExposureThresholdControl, ob_tof_exposure_threshold_control, TOF_EXPOSURE_THRESHOLD_CONTROL;

/**
 * @brief Sync mode
 *
 */
typedef enum {
    /**
     * @brief Close synchronize mode
     * @brief Single device, neither process input trigger signal nor output trigger signal
     * @brief Each Sensor in a single device automatically triggers
     */
    OB_SYNC_MODE_CLOSE = 0x00,

    /**
     * @brief Standalone synchronize mode
     * @brief Single device, neither process input trigger signal nor output trigger signal
     * @brief Inside single device, RGB as Major sensor: RGB -> IR/Depth/TOF
     */
    OB_SYNC_MODE_STANDALONE = 0x01,

    /**
     * @brief Primary synchronize mode
     * @brief Primary device. Ignore process input trigger signal, only output trigger signal to secondary devices.
     * @brief Inside single device, RGB as Major sensor: RGB -> IR/Depth/TOF
     *
     */
    OB_SYNC_MODE_PRIMARY = 0x02,

    /**
     * @brief Secondary synchronize mode
     * @brief Secondary device. Both process input trigger signal and output trigger signal to other devices.
     * @brief Different sensors in a single devices receive trigger signals respectively：ext trigger -> RGB && ext trigger -> IR/Depth/TOF
     *
     * @attention With the current Gemini 2 device set to this mode, each Sensor receives the first external trigger signal
     *     after the stream is turned on and starts timing self-triggering at the set frame rate until the stream is turned off
     *
     */
    OB_SYNC_MODE_SECONDARY = 0x03,

    /**
     * @brief MCU Primary synchronize mode
     * @brief Primary device. Ignore process input trigger signal, only output trigger signal to secondary devices.
     * @brief Inside device, MCU is the primary signal source:  MCU -> RGB && MCU -> IR/Depth/TOF
     */
    OB_SYNC_MODE_PRIMARY_MCU_TRIGGER = 0x04,

    /**
     * @brief IR Primary synchronize mode
     * @brief Primary device. Ignore process input trigger signal, only output trigger signal to secondary devices.
     * @brief Inside device, IR is the primary signal source: IR/Depth/TOF -> RGB
     */
    OB_SYNC_MODE_PRIMARY_IR_TRIGGER = 0x05,

    /**
     * @brief Software trigger synchronize mode
     * @brief Host, triggered by software control (receive the upper computer command trigger), at the same time to the trunk output trigger signal
     * @brief Different sensors in a single machine receive trigger signals respectively: soft trigger -> RGB && soft trigger -> IR/Depth/TOF
     * @attention Support product: Gemini2
     */
    OB_SYNC_MODE_PRIMARY_SOFT_TRIGGER = 0x06,

    /**
     * @brief Software trigger synchronize mode as secondary device
     * @brief The slave receives the external trigger signal (the external trigger signal comes from the soft trigger host) and outputs the trigger signal to
     * the external relay.
     * @brief Different sensors in a single machine receive trigger signals respectively：ext trigger -> RGB && ext  trigger -> IR/Depth/TOF
     *
     */
    OB_SYNC_MODE_SECONDARY_SOFT_TRIGGER = 0x07,

    /**
     * @brief Unknown type
     *
     */
    OB_SYNC_MODE_UNKNOWN = 0xff,

} OBSyncMode,
    ob_sync_mode, OB_SYNC_MODE;

// Alias for compatibility with older versions of naming
#define OB_SYNC_STOP OB_SYNC_MODE_CLOSE
#define OB_SYNC_SINGLE_MODE OB_SYNC_MODE_STANDALONE
#define OB_SYNC_ONLINE_HOST_MODE OB_SYNC_MODE_PRIMARY
#define OB_SYNC_ONLINE_SLAVE_MODE OB_SYNC_MODE_SECONDARY
#define OB_SYNC_ONLY_MCU_MODE OB_SYNC_MODE_PRIMARY_MCU_TRIGGER
#define OB_SYNC_ONLY_IR_MODE OB_SYNC_MODE_PRIMARY_IR_TRIGGER

/**
 * @brief Device synchronization configuration
 */
typedef struct {
    /**
     * @brief Device synchronize mode
     */
    OBSyncMode syncMode;

    /**
     * @brief IR Trigger signal input delay: Used to configure the delay between the IR/Depth/TOF Sensor receiving the trigger signal and starting exposure,
     * Unit: microsecond
     *
     * @attention This parameter is invalid when the synchronization MODE is set to @ref OB_SYNC_MODE_PRIMARY_IR_TRIGGER
     */
    uint16_t irTriggerSignalInDelay;

    /**
     * @brief RGB trigger signal input delay is used to configure the delay from the time when an RGB Sensor receives the trigger signal to the time when the
     * exposure starts. Unit: microsecond
     *
     * @attention This parameter is invalid when the synchronization MODE is set to @ref OB_SYNC_MODE_PRIMARY
     */
    uint16_t rgbTriggerSignalInDelay;

    /**
     * @brief Device trigger signal output delay, used to control the delay configuration of the host device to output trigger signals or the slave device to
     * output trigger signals. Unit: microsecond
     *
     * @attention This parameter is invalid when the synchronization MODE is set to @ref OB_SYNC_MODE_CLOSE or @ref OB_SYNC_MODE_STANDALONE
     */
    uint16_t deviceTriggerSignalOutDelay;

    /**
     * @brief The device trigger signal output polarity is used to control the polarity configuration of the trigger signal output from the host device or the
     * trigger signal output from the slave device
     * @brief 0: forward pulse; 1: negative pulse
     *
     * @attention This parameter is invalid when the synchronization MODE is set to @ref OB_SYNC_MODE_CLOSE or @ref OB_SYNC_MODE_STANDALONE
     */
    uint16_t deviceTriggerSignalOutPolarity;

    /**
     * @brief MCU trigger frequency, used to configure the output frequency of MCU trigger signal in MCU master mode, unit: Hz
     * @brief This configuration will directly affect the image output frame rate of the Sensor. Unit: FPS （frame pre second）
     *
     * @attention This parameter is invalid only when the synchronization MODE is set to @ref OB_SYNC_MODE_PRIMARY_MCU_TRIGGER
     */
    uint16_t mcuTriggerFrequency;

    /**
     * @brief Device number. Users can mark the device with this number
     */
    uint16_t deviceId;
} OBDeviceSyncConfig, ob_device_sync_config, OB_DEVICE_SYNC_CONFIG;

// 别名，用于兼容旧版本命名
#define OBMultiDeviceSyncConfig OBDeviceSyncConfig
#define ob_multi_device_sync_config ob_device_sync_config
#define OB_MULTI_DEVICE_SYNC_CONFIG OB_DEVICE_SYNC_CONFIG
// #define tofPhaseDelay irTriggerSignalInDelay
// #define rgbPhaseDelay rgbTriggerSignalInDelay
// #define outPhaseDelay deviceTriggerSignalOutDelay
// #define outOCPolarity deviceTriggerSignalOutPolarity
// #define mcuHostFps mcuTriggerFrequency
// #define curDevId deviceId

/**
 * @brief Depth work mode
 *
 */
typedef struct {
    uint8_t checksum[16];  // Checksum of work mode
    char    name[32];      // Name of work mode
} OBDepthWorkMode, ob_depth_work_mode;

/**
 * @brief Control command protocol version number
 */
typedef struct {
    uint8_t major;  ///< \if English Major version number \else 主版本号 \endif
    uint8_t minor;  ///< \if English Minor version number \else 次版本号 \endif
    uint8_t patch;  ///< \if English Patch version number \else 补丁版本 \endif
} OBProtocolVersion, ob_protocol_version;

/**
 * @brief Command version associate with property id
 */
typedef enum {
    OB_CMD_VERSION_V0 = (uint16_t)0,  // version 1.0
    OB_CMD_VERSION_V1 = (uint16_t)1,  // version 2.0
    OB_CMD_VERSION_V2 = (uint16_t)2,  // version 3.0
    OB_CMD_VERSION_V3 = (uint16_t)3,  // version 4.0

    OB_CMD_VERSION_NOVERSION = (uint16_t)0xfffe,
    OB_CMD_VERSION_INVALID   = (uint16_t)0xffff,  // Invalid version
} OB_CMD_VERSION,
    OBCmdVersion, ob_cmd_version;

/**
 * Internal api, publish future.
 * Description: Data type match OBCmdVersion of one propertyId
 * 1. propertyId has multiple OBCmdVersion; different OBCmdVersion of this propertyId has different data type;
 *    propertyId && OBCmdVersion match only one data type.
 * 2. itemCount is the number of data type contain in data bytes.
 * 3. C language and C++ is difference.
 *
 * C language:
 * data's type is an uint8_t pointer, user parse data to destionation type.
 * itemTypeSize == 1，dataSize == itemCount；
 *
 * C++:
 * data's type is the propertyId && OBCmdVersion's data type.
 * itemTypeSize = sizeof(T)，itemCount = dataSize / itemTypeSize;
 */
typedef struct OBDataBundle {
    OBCmdVersion cmdVersion;  // propertyId's OBCmdVersion

    void *   data;          // Data contain which contain itemCount of element. void *data = new T[itemCount];
    uint32_t dataSize;      // Data size. unit: byte. dataSize == itemTypeSize * itemCount
    uint32_t itemTypeSize;  // Size of data item. C language, itemTypeSize = 1, C++: itemTypeSize = sizeof(T)
    uint32_t itemCount;     // Count of data item. itemCount = dataSize / itemTypeSize; 0 == dataSize % itemTypeSize;
} OBDataBundle, ob_data_bundle;

/**
 * @brief IP address configuration for network devices (IPv4)
 *
 */
typedef struct {
    uint16_t dhcp;        // dhcp dynamic ip configuration switch; 0: off; 1: on
    uint8_t  address[4];  // ip address (big-end mode, e.g. 192.168.1.1, then address[0]==192)
    uint8_t  mask[4];     // Subnet Mask (Big End Mode)
    uint8_t  gateway[4];  // Gateway (big-end mode)
} OBDeviceIpAddrConfig, ob_device_ip_addr_config, DEVICE_IP_ADDR_CONFIG;

/**
 * @brief Device communication mode
 *
 */
typedef enum {
    OB_COMM_USB = 0x00,  // USB
    OB_COMM_NET = 0x01,  // Ethernet
} OBCommunicationType,
    ob_communication_type, OB_COMMUNICATION_TYPE;

/**
 * @brief USB power status
 *
 */
typedef enum {
    OB_USB_POWER_NO_PLUGIN = 0,  // no plugin
    OB_USB_POWER_5V_0A9    = 1,  // 5V/0.9A
    OB_USB_POWER_5V_1A5    = 2,  // 5V/1.5A
    OB_USB_POWER_5V_3A0    = 3,  // 5V/3.0A
} OBUSBPowerState,
    ob_usb_power_state;

/**
 * @brief DC power status
 *
 */
typedef enum {
    OB_DC_POWER_NO_PLUGIN = 0,  // no plugin
    OB_DC_POWER_PLUGIN    = 1,  // plugin
} OBDCPowerState,
    ob_dc_power_state;

/**
 * @brief Rotate degree
 *
 */
typedef enum {
    OB_ROTATE_DEGREE_0   = 0,    // Rotate 0
    OB_ROTATE_DEGREE_90  = 90,   // Rotate 90
    OB_ROTATE_DEGREE_180 = 180,  // Rotate 180
    OB_ROTATE_DEGREE_270 = 270,  // Rotate 270
} ob_rotate_degree_type,
    OBRotateDegreeType;

/**
 * @brief Power line frequency mode，for Color camera anti-flicker configuration
 *
 */
typedef enum {
    OB_POWER_LINE_FREQ_MODE_CLOSE = 0,  // close
    OB_POWER_LINE_FREQ_MODE_50HZ  = 1,  // 50Hz
    OB_POWER_LINE_FREQ_MODE_60HZ  = 2,  // 60Hz
} ob_power_line_freq_mode,
    OBPowerLineFreqMode;

/**
 * @brief Frame aggregate output mode
 *
 */
typedef enum {
    /**
     * @brief Only FrameSet that contains all types of data frames will be output
     *
     */
    OB_FRAME_AGGREGATE_OUTPUT_FULL_FRAME_REQUIRE = 0,

    /**
     *
     * @brief Frame aggregate output mode
     * @brief Suitable for Color using H264, H265 and other inter-frame encoding format open stream
     *
     * @attention In this mode, the user may return null when getting a non-Color type data frame from the acquired FrameSet
     */
    OB_FRAME_AGGREGATE_OUTPUT_COLOR_FRAME_REQUIRE,

    /**
     *
     * @brief FrameSet for any case will be output
     *
     * @attention In this mode, the user may return null when getting the specified type of data frame from the acquired FrameSet
     *
     *
     */
    OB_FRAME_AGGREGATE_OUTPUT_ANY_SITUATION,
} OB_FRAME_AGGREGATE_OUTPUT_MODE,
    OBFrameAggregateOutputMode, ob_frame_aggregate_output_mode;

/**
 * @brief Point cloud coordinate system type
 *
 */
typedef enum {
    OB_LEFT_HAND_COORDINATE_SYSTEM  = 0,
    OB_RIGHT_HAND_COORDINATE_SYSTEM = 1,
} OB_COORDINATE_SYSTEM_TYPE,
    OBCoordinateSystemType, ob_coordinate_system_type;

/**
 * @brief file transfer callback
 * @param state Transmission status
 * @param message Transfer status information
 * @param state Transfer progress percentage
 * @param user_data user-defined data
 */
typedef void (*ob_file_send_callback)(ob_file_tran_state state, const char *message, uint8_t percent, void *user_data);

/**
 * @brief Firmware upgrade callback
 * @param state upgrade status
 * @param message upgrade status information
 * @param state upgrade progress percentage
 * @param user_data user-defined data
 */
typedef void (*ob_device_upgrade_callback)(ob_upgrade_state state, const char *message, uint8_t percent, void *user_data);

/**
 * @brief device status callback
 * @param state device status
 * @param message Device Status Information
 * @param user_data user-defined data
 */
typedef void (*ob_device_state_callback)(ob_device_state state, const char *message, void *user_data);

/**
 * @brief Callback for writing data
 * @param state write data status
 * @param percent Write data percentage
 * @param user_data user-defined data
 */
typedef void (*ob_set_data_callback)(ob_data_tran_state state, uint8_t percent, void *user_data);

/**
 * @brief read data callback
 * @param state read data status
 * @param dataChunk read the returned data block
 * @param user_data user-defined data
 */

typedef void (*ob_get_data_callback)(ob_data_tran_state state, ob_data_chunk *dataChunk, void *user_data);

/**
 * @brief Media status callbacks (recording and playback)
 * @param state condition
 * @param user_data user-defined data
 */
typedef void (*ob_media_state_callback)(ob_media_state state, void *user_data);

/**
 * @brief Device change (up and down) callback
 * @param removed List of deleted (dropped) devices
 * @param added List of added (online) devices
 * @param user_data user-defined data
 */
typedef void (*ob_device_changed_callback)(ob_device_list *removed, ob_device_list *added, void *user_data);

/**
 * @brief dataframe callback
 * @param frame Data Frame
 * @param user_data user-defined data
 */
typedef void (*ob_frame_callback)(ob_frame *frame, void *user_data);
#define ob_filter_callback ob_frame_callback
#define ob_playback_callback ob_frame_callback

/**
 * @brief dataframe collection callback
 * @param frameset collection of dataframes
 * @param user_data user-defined data
 */

typedef void (*ob_frameset_callback)(ob_frame *frameset, void *user_data);

/**
 * @brief Customize the delete callback
 * @param buffer Data that needs to be deleted
 * @param context user-defined data
 */
typedef void(ob_frame_destroy_callback)(void *buffer, void *context);

/**
 * @brief Check sensor_type is IR sensor, true: IR sensor, false: no IR sensor
 */
#define is_ir_sensor(sensor_type) (sensor_type == OB_SENSOR_IR || sensor_type == OB_SENSOR_IR_LEFT || sensor_type == OB_SENSOR_IR_RIGHT)
#define isIRSensor is_ir_sensor

/**
 * @brief Check stream_type is IR stream, true: IR stream, false: no IR stream
 */
#define is_ir_stream(stream_type) (stream_type == OB_STREAM_IR || stream_type == OB_STREAM_IR_LEFT || stream_type == OB_STREAM_IR_RIGHT)
#define isIRStream is_ir_stream

/**
 * @brief Check frame_type is IR frame, true: IR frame, false: no IR frame
 */
#define is_ir_frame(frame_type) (frame_type == OB_FRAME_IR || frame_type == OB_FRAME_IR_LEFT || frame_type == OB_FRAME_IR_RIGHT)
#define isIRFrame is_ir_frame

/**
 * @brief The default Decrypt Key
 */
#define OB_DEFAULT_DECRYPT_KEY (nullptr)

#ifdef __cplusplus
}
#endif

#pragma pack(pop)
