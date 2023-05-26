// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2020 Orbbec  Corporation. All Rights Reserved.
#ifdef OB_SENSOR_SDK_DEVELOPER
#include "libobsensor/internal/InternalProperty.h"
#else  // not define OB_SENSOR_SDK_DEVELOPER
#ifndef _OB_PROPERTY_H_
#define _OB_PROPERTY_H_

#include "ObTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration value describing all attribute control commands of the device
 */
typedef enum {
    OB_PROP_LDP_BOOL                              = 2,   // LDP switch
    OB_PROP_LASER_BOOL                            = 3,   // Laser switch
    OB_PROP_LASER_PULSE_WIDTH_INT                 = 4,   // Read and write laser Time value (pulse width)
    OB_PROP_LASER_CURRENT_FLOAT                   = 5,   // Laser current
    OB_PROP_FLOOD_BOOL                            = 6,   // IR flood switch
    OB_PROP_FLOOD_LEVEL_INT                       = 7,   // IR flood level
    OB_PROP_DEPTH_MIRROR_BOOL                     = 14,  // Depth mirror
    OB_PROP_DEPTH_FLIP_BOOL                       = 15,  // Depth flip
    OB_PROP_DEPTH_POSTFILTER_BOOL                 = 16,  // Depth Postfilter
    OB_PROP_DEPTH_HOLEFILTER_BOOL                 = 17,  // Depth Holefilter
    OB_PROP_IR_MIRROR_BOOL                        = 18,  // IR mirror
    OB_PROP_IR_FLIP_BOOL                          = 19,  // IR flip
    OB_PROP_MIN_DEPTH_INT                         = 22,  // Minimum depth threshold
    OB_PROP_MAX_DEPTH_INT                         = 23,  // Maximum depth threshold
    OB_PROP_DEPTH_SOFT_FILTER_BOOL                = 24,  // Software filter switch
    OB_PROP_LDP_STATUS_BOOL                       = 32,  // LDP status
    OB_PROP_DEPTH_MAX_DIFF_INT                    = 40,  // soft filter maxdiff param */
    OB_PROP_DEPTH_MAX_SPECKLE_SIZE_INT            = 41,  // soft filter maxSpeckleSize */
    OB_PROP_DEPTH_ALIGN_HARDWARE_BOOL             = 42,  // Hardware d2c is on
    OB_PROP_TIMESTAMP_OFFSET_INT                  = 43,  // Timestamp adjustment
    OB_PROP_HARDWARE_DISTORTION_SWITCH_BOOL       = 61,  // Hardware distortion switch Rectify
    OB_PROP_FAN_WORK_MODE_INT                     = 62,  // Fan mode switch
    OB_PROP_DEPTH_ALIGN_HARDWARE_MODE_INT         = 63,  // Multi-resolution D2C mode
    OB_PROP_ANTI_COLLUSION_ACTIVATION_STATUS_BOOL = 64,  // Anti_collusion activation status
    OB_PROP_DEPTH_PRECISION_LEVEL_INT = 75,  // The depth precision level, which may change the depth frame data unit, needs to be confirmed through the
                                             // ValueScale interface of DepthFrame
    OB_PROP_TOF_FILTER_RANGE_INT = 76,       // tof filter range configuration
    OB_PROP_LASER_MODE_INT       = 79,       // laser mode, the firmware terminal currently only returns 1: IR Drive, 2: Torch

    OB_PROP_RECTIFY2_BOOL        = 80,  // brt2r-rectify function switch (brt2r is a special module on mx6600), 0: Disable, 1: Rectify Enable
    OB_PROP_COLOR_MIRROR_BOOL    = 81,  // Color mirror
    OB_PROP_COLOR_FLIP_BOOL      = 82,  // Color flip
    OB_PROP_INDICATOR_LIGHT_BOOL = 83,  // Indicator switch, 0: Disable, 1: Enable
    OB_PROP_DISPARITY_TO_DEPTH_BOOL =
        85,  // Disparity to depth switch, 0: off, the depth stream outputs the disparity map; 1. On, the depth stream outputs the depth map.
    OB_PROP_BRT_BOOL                   = 86,  // BRT function switch (anti-background interference), 0: Disable, 1: Enable
    OB_PROP_WATCHDOG_BOOL              = 87,  // Watchdog function switch, 0: Disable, 1: Enable
    OB_PROP_EXTERNAL_SIGNAL_RESET_BOOL = 88,  // External signal trigger restart function switch, 0: Disable, 1: Enable
    OB_PROP_HEARTBEAT_BOOL             = 89,  // Heartbeat monitoring function switch, 0: Disable, 1: Enable
    OB_PROP_DEPTH_CROPPING_MODE_INT    = 90,  // Depth cropping mode device: OB_DEPTH_CROPPING_MODE
    OB_PROP_D2C_PREPROCESS_BOOL        = 91,  // D2C preprocessing switch (such as RGB cropping), 0: off, 1: on
    OB_PROP_RGB_CUSTOM_CROP_BOOL       = 94,  // Custom RGB cropping switch, 0 is off, 1 is on custom cropping, and the ROI cropping area is issued

    OB_PROP_DEVICE_WORK_MODE_INT                = 95,   // Device operating mode (power consumption)
    OB_PROP_DEVICE_COMMUNICATION_TYPE_INT       = 97,   // Device communication type, 0: USB; 1: Ethernet(RTSP)
    OB_PROP_SWITCH_IR_MODE_INT                  = 98,   //   Switch IR mode, 0: IR active mode, 1: IR passive mode
    OB_PROP_LASER_ENERGY_LEVEL_INT              = 99,   //   Laser energy level
    OB_PROP_LDP_MEASURE_DISTANCE_INT            = 100,  //   LDP's measure distance, unit: mm
    OB_PROP_TIMER_RESET_SIGNAL_BOOL             = 104,  //   Reset device time to zero
    OB_PROP_TIMER_RESET_TRIGGLE_OUT_ENABLE_BOOL = 105,  //   Enable send reset device time signal to other device. true: enable, false: disable
    OB_PROP_TIMER_RESET_DELAY_US_INT            = 106,  //   Delay to reset device time, unit: us
    OB_PROP_CAPTURE_IMAGE_SIGNAL_BOOL           = 107,  //   Signal to capture image
    OB_PROP_IR_RIGHT_MIRROR_BOOL                = 112,  //   Right IR sensor mirror state
    OB_PROP_CAPTURE_IMAGE_FRAME_NUMBER_INT      = 113,  //   Number frame to capture once a 'OB_PROP_CAPTURE_IMAGE_SIGNAL_BOOL' effect. range: [1, 255]
    OB_PROP_IR_RIGHT_FLIP_BOOL                  = 114,  //   Right IR sensor flip state. true: flip image, false: origin, default: false
    OB_PROP_COLOR_ROTATE_INT                    = 115,  //   Color sensor rotation, angle{0, 90, 180, 270}
    OB_PROP_IR_ROTATE_INT                       = 116,  //   IR/Left-IR sensor rotation, angle{0, 90, 180, 270}
    OB_PROP_IR_RIGHT_ROTATE_INT                 = 117,  //   Right IR sensor rotation, angle{0, 90, 180, 270}
    OB_PROP_DEPTH_ROTATE_INT                    = 118,  //   Depth sensor rotation, angle{0, 90, 180, 270}
    OB_PROP_LASER_HW_ENERGY_LEVEL_INT = 119,  // Get hardware laser energy level which real state of laser element. OB_PROP_LASER_ENERGY_LEVEL_INT（99）will
                                              // effect this command which it setting and changed the hardware laser energy level.
    OB_PROP_USB_POWER_STATE_INT          = 121,  // USB's power state, enum type: OBUSBPowerState
    OB_PROP_DC_POWER_STATE_INT           = 122,  // DC's power state, enum type: OBDCPowerState
    OB_PROP_SYNC_SIGNAL_TRIGGLE_OUT_BOOL = 130,  // Multi-DeviceSync synchronized signal triggle out is enable state. true: enable, false: disable

    OB_STRUCT_BASELINE_CALIBRATION_PARAM     = 1002,  // Baseline calibration parameters
    OB_STRUCT_DEVICE_TEMPERATURE             = 1003,  // Device temperature information
    OB_STRUCT_TOF_EXPOSURE_THRESHOLD_CONTROL = 1024,  // TOF exposure threshold range
    OB_STRUCT_DEVICE_SERIAL_NUMBER           = 1035,  // get/set serial number
    OB_STRUCT_DEVICE_TIME                    = 1037,  // get/set device time
    OB_STRUCT_MULTI_DEVICE_SYNC_CONFIG       = 1038,  // Multi-device synchronization mode and parameter configuration
    OB_STRUCT_RGB_CROP_ROI                   = 1040,  // RGB cropping ROI
    OB_STRUCT_DEVICE_IP_ADDR_CONFIG          = 1041,  // Device IP address configuration
    OB_STRUCT_CURRENT_DEPTH_ALG_MODE         = 1043,  // The current camera depth mode
    OB_STRUCT_DEPTH_PRECISION_SUPPORT_LIST   = 1045,  // A list of depth accuracy levels, returning an array of uin16_t, corresponding to the enumeration
                                                      // definitions of the accuracy classes

    OB_PROP_COLOR_AUTO_EXPOSURE_BOOL         = 2000,  // Color camera auto exposure
    OB_PROP_COLOR_EXPOSURE_INT               = 2001,  // Color camera exposure adjustment
    OB_PROP_COLOR_GAIN_INT                   = 2002,  // Color camera gain adjustment
    OB_PROP_COLOR_AUTO_WHITE_BALANCE_BOOL    = 2003,  // Color camera automatic white balance
    OB_PROP_COLOR_WHITE_BALANCE_INT          = 2004,  // Color camera white balance adjustment
    OB_PROP_COLOR_BRIGHTNESS_INT             = 2005,  // Color camera brightness adjustment
    OB_PROP_COLOR_SHARPNESS_INT              = 2006,  // Color camera sharpness adjustment
    OB_PROP_COLOR_SATURATION_INT             = 2008,  // Color camera saturation adjustment
    OB_PROP_COLOR_CONTRAST_INT               = 2009,  // Color camera contrast adjustment
    OB_PROP_COLOR_GAMMA_INT                  = 2010,  // Color camera gamma adjustment
    OB_PROP_COLOR_ROLL_INT                   = 2011,  // Color camera image rotation
    OB_PROP_COLOR_AUTO_EXPOSURE_PRIORITY_INT = 2012,  // Color camera auto exposure priority
    OB_PROP_COLOR_BACKLIGHT_COMPENSATION_INT = 2013,  // Color camera brightness compensation
    OB_PROP_COLOR_HUE_INT                    = 2014,  // color camera color tint
    OB_PROP_COLOR_POWER_LINE_FREQUENCY_INT   = 2015,  // Color Camera Power Line Frequency
    OB_PROP_DEPTH_AUTO_EXPOSURE_BOOL         = 2016,  // Automatic exposure of depth camera (infrared camera will be set synchronously under some models of
                                                      // devices)
    OB_PROP_DEPTH_EXPOSURE_INT = 2017,                // Depth camera exposure adjustment (infrared cameras will be set synchronously under some models of
                                                      // devices)
    OB_PROP_DEPTH_GAIN_INT             = 2018,        // Depth camera gain adjustment (infrared cameras will be set synchronously under some models of devices)
    OB_PROP_GYRO_ODR_INT               = 2021,        // get/set current gyro sample rate
    OB_PROP_ACCEL_ODR_INT              = 2022,        // get/set current accel sample rate
    OB_PROP_GYRO_FULL_SCALE_INT        = 2023,        // get/set current gyro full scale range
    OB_PROP_ACCEL_FULL_SCALE_INT       = 2024,        // get/set current accel full scale range
    OB_PROP_IR_AUTO_EXPOSURE_BOOL      = 2025,        // Infrared camera auto exposure (depth camera will be set synchronously under some models of devices)
    OB_PROP_IR_EXPOSURE_INT            = 2026,        // Infrared camera exposure adjustment (some models of devices will set the depth camera synchronously)
    OB_PROP_IR_GAIN_INT                = 2027,  // Infrared camera gain adjustment (the depth camera will be set synchronously under some models of devices)
    OB_PROP_IR_CHANNEL_DATA_SOURCE_INT = 2028,  // Select Infrared camera data source channel. If not support throw exception. 0 : IR stream from IR Left
                                                // sensor; 1 : IR stream from IR Left sensor;
    OB_PROP_DEPTH_RM_FILTER_BOOL = 2029,        // Depth effect dedistortion, true: on, false: off. mutually exclusive with D2C function, RM_Filter disable When
                                                // hardware or software D2C is enabled.

    OB_PROP_SDK_DISPARITY_TO_DEPTH_BOOL = 3004,  // Software disparity to depth
    OB_PROP_SDK_DEPTH_FRAME_UNPACK_BOOL =
        3007,  // Depth data unpacking function switch (each open stream will be turned on by default, support RLE/Y10/Y11/Y12/Y14 format)
    OB_PROP_SDK_IR_FRAME_UNPACK_BOOL =
        3008,  // IR data unpacking function switch (each current will be turned on by default, support RLE/Y10/Y11/Y12/Y14 format)
    OB_PROP_SDK_ACCEL_FRAME_TRANSFORMED_BOOL = 3009,  // Accel data conversion function switch (on by default)
    OB_PROP_SDK_GYRO_FRAME_TRANSFORMED_BOOL  = 3010,  // Gyro data conversion function switch (on by default)
    OB_PROP_SDK_IR_LEFT_FRAME_UNPACK_BOOL =
        3011,  // IR[LEFT] data unpacking function switch (each current will be turned on by default, support RLE/Y10/Y11/Y12/Y14 format)
    OB_PROP_SDK_IR_RIGHT_FRAME_UNPACK_BOOL =
        3012,  // IR[RIGHT] data unpacking function switch (each current will be turned on by default, support RLE/Y10/Y11/Y12/Y14 format)
    OB_RAW_DATA_CAMERA_CALIB_JSON_FILE = 4029,  // Calibration JSON file read from device (Femto Mega, read only)
} OBPropertyID,
    ob_property_id;

/**
 * @brief The data type used to describe all property settings
 */
typedef enum OBPropertyType {
    OB_BOOL_PROPERTY   = 0,  // Bool property
    OB_INT_PROPERTY    = 1,  // int property
    OB_FLOAT_PROPERTY  = 2,  // float property
    OB_STRUCT_PROPERTY = 3,  // struct property
} OBPropertyType,
    ob_property_type;

/**
 * @brief Used to describe the characteristics of each property
 */
typedef struct OBPropertyItem {
    OBPropertyID     id;          //  Property id
    const char *     name;        //  Property name
    OBPropertyType   type;        //  Property type
    OBPermissionType permission;  //  Property read and write permission
} OBPropertyItem, ob_property_item;

#ifdef __cplusplus
}
#endif

#endif  // _OB_PROPERTY_H_
#endif  // OB_SENSOR_SDK_DEVELOPER
