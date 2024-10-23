C API参考
============================================
宏定义
--------------------------------------------
.. doxygendefine:: OB_EXTENSION_API
   :project: OrbbecSdk

.. doxygendefine:: DEPRECATED
   :project: OrbbecSdk

.. doxygendefine:: ob_filter_callback
   :project: OrbbecSdk

.. doxygendefine:: ob_playback_callback
   :project: OrbbecSdk

结构体
--------------------------------------------
.. doxygenstruct:: ob_error
   :project: OrbbecSdk

.. doxygenstruct:: OBAccelValue
   :project: OrbbecSdk

.. doxygenstruct:: OBBoolPropertyRange
   :project: OrbbecSdk

.. doxygenstruct:: OBCameraDistortion
   :project: OrbbecSdk

.. doxygenstruct:: OBCameraIntrinsic
   :project: OrbbecSdk

.. doxygenstruct:: OBCameraParam
   :project: OrbbecSdk

.. doxygenstruct:: OBColorPoint
   :project: OrbbecSdk

.. doxygenstruct:: OBD2CTransform
   :project: OrbbecSdk

.. doxygenstruct:: OBDataChunk
   :project: OrbbecSdk

.. doxygenstruct:: OBDeviceTemperature
   :project: OrbbecSdk

.. doxygenstruct:: OBFloatPropertyRange
   :project: OrbbecSdk

.. doxygenstruct:: OBIntPropertyRange
   :project: OrbbecSdk

.. doxygenstruct:: OBMultiDeviceSyncConfig
   :project: OrbbecSdk

.. doxygenstruct:: OBPoint
   :project: OrbbecSdk

.. doxygenstruct:: OBPropertyItem
   :project: OrbbecSdk

.. doxygenstruct:: OBRect
   :project: OrbbecSdk

.. doxygenstruct:: OBTofExposureThresholdControl
   :project: OrbbecSdk

枚举
--------------------------------------------
.. doxygenenum:: OBPermissionType
   :project: OrbbecSdk

.. doxygenenum:: OBStatus
   :project: OrbbecSdk

.. doxygenenum:: OBLogSeverity
   :project: OrbbecSdk

.. doxygenenum:: OBExceptionType
   :project: OrbbecSdk

.. doxygenenum:: OBSensorType
   :project: OrbbecSdk

.. doxygenenum:: OBStreamType
   :project: OrbbecSdk

.. doxygenenum:: OBFrameType
   :project: OrbbecSdk

.. doxygenenum:: OBFormat
   :project: OrbbecSdk

.. doxygenenum:: OBUpgradeState
   :project: OrbbecSdk

.. doxygenenum:: OBFileTranState
   :project: OrbbecSdk

.. doxygenenum:: OBDataTranState
   :project: OrbbecSdk

.. doxygenenum:: OBAlignMode
   :project: OrbbecSdk

.. doxygenenum:: OBConvertFormat
   :project: OrbbecSdk

.. doxygenenum:: OBGyroSampleRate
   :project: OrbbecSdk

.. doxygenenum:: OBGyroFullScaleRange
   :project: OrbbecSdk

.. doxygenenum:: OBAccelFullScaleRange
   :project: OrbbecSdk

.. doxygenenum:: OBDeviceState
   :project: OrbbecSdk

.. doxygenenum:: OBDepthCroppingMode
   :project: OrbbecSdk

.. doxygenenum:: OBDeviceType
   :project: OrbbecSdk

.. doxygenenum:: OBMediaType
   :project: OrbbecSdk

.. doxygenenum:: OBMediaState
   :project: OrbbecSdk

.. doxygenenum:: OBDepthPrecisionLevel
   :project: OrbbecSdk

.. doxygenenum:: OBTofFilterRange
   :project: OrbbecSdk

.. doxygenenum:: OBSyncMode
   :project: OrbbecSdk

.. doxygenenum:: OBPropertyID
   :project: OrbbecSdk

.. doxygenenum:: OBPropertyType
   :project: OrbbecSdk

类型定义
--------------------------------------------
.. doxygentypedef:: ob_context
   :project: OrbbecSdk

.. doxygentypedef:: ob_device
   :project: OrbbecSdk

.. doxygentypedef:: ob_device_info
   :project: OrbbecSdk

.. doxygentypedef:: ob_device_list
   :project: OrbbecSdk

.. doxygentypedef:: ob_camera_param_list
   :project: OrbbecSdk

.. doxygentypedef:: ob_sensor
   :project: OrbbecSdk

.. doxygentypedef:: ob_sensor_list
   :project: OrbbecSdk

.. doxygentypedef:: ob_stream_profile
   :project: OrbbecSdk

.. doxygentypedef:: ob_stream_profile_list
   :project: OrbbecSdk

.. doxygentypedef:: ob_frame
   :project: OrbbecSdk

.. doxygentypedef:: ob_filter
   :project: OrbbecSdk

.. doxygentypedef:: ob_pipeline
   :project: OrbbecSdk

.. doxygentypedef:: ob_config
   :project: OrbbecSdk

.. doxygentypedef:: ob_recorder
   :project: OrbbecSdk

.. doxygentypedef:: ob_playback
   :project: OrbbecSdk

.. doxygentypedef:: ob_permission_type
   :project: OrbbecSdk

.. doxygentypedef:: ob_status
   :project: OrbbecSdk

.. doxygentypedef:: ob_log_severity
   :project: OrbbecSdk

.. doxygentypedef:: DEVICE_LOG_SEVERITY_LEVEL
   :project: OrbbecSdk

.. doxygentypedef:: OBDeviceLogSeverityLevel
   :project: OrbbecSdk

.. doxygentypedef:: ob_device_log_severity_level
   :project: OrbbecSdk

.. doxygentypedef:: ob_exception_type
   :project: OrbbecSdk

.. doxygentypedef:: ob_error
   :project: OrbbecSdk

.. doxygentypedef:: ob_sensor_type
   :project: OrbbecSdk

.. doxygentypedef:: ob_stream_type
   :project: OrbbecSdk

.. doxygentypedef:: ob_frame_type
   :project: OrbbecSdk

.. doxygentypedef:: ob_format
   :project: OrbbecSdk

.. doxygentypedef:: ob_upgrade_state
   :project: OrbbecSdk

.. doxygentypedef:: ob_file_tran_state
   :project: OrbbecSdk

.. doxygentypedef:: ob_data_tran_state
   :project: OrbbecSdk

.. doxygentypedef:: ob_data_chunk
   :project: OrbbecSdk

.. doxygentypedef:: ob_int_property_range
   :project: OrbbecSdk

.. doxygentypedef:: ob_float_property_range
   :project: OrbbecSdk

.. doxygentypedef:: ob_bool_property_range
   :project: OrbbecSdk

.. doxygentypedef:: ob_camera_intrinsic
   :project: OrbbecSdk

.. doxygentypedef:: ob_camera_distortion
   :project: OrbbecSdk

.. doxygentypedef:: ob_d2c_transform
   :project: OrbbecSdk

.. doxygentypedef:: ob_camera_param
   :project: OrbbecSdk

.. doxygentypedef:: ob_align_mode
   :project: OrbbecSdk

.. doxygentypedef:: ob_rect
   :project: OrbbecSdk

.. doxygentypedef:: ob_convert_format
   :project: OrbbecSdk

.. doxygentypedef:: ob_gyro_sample_rate
   :project: OrbbecSdk

.. doxygentypedef:: OBAccelSampleRate
   :project: OrbbecSdk

.. doxygentypedef:: ob_accel_sample_rate
   :project: OrbbecSdk

.. doxygentypedef:: OB_SAMPLE_RATE
   :project: OrbbecSdk

.. doxygentypedef:: ob_gyro_full_scale_range
   :project: OrbbecSdk

.. doxygentypedef:: OB_GYRO_FULL_SCALE_RANGE
   :project: OrbbecSdk

.. doxygentypedef:: ob_accel_full_scale_range
   :project: OrbbecSdk

.. doxygentypedef:: OB_ACCEL_FULL_SCALE_RANGE
   :project: OrbbecSdk

.. doxygentypedef:: OBGyroValue
   :project: OrbbecSdk

.. doxygentypedef:: ob_accel_value
   :project: OrbbecSdk

.. doxygentypedef:: ob_gyro_value
   :project: OrbbecSdk

.. doxygentypedef:: ob_device_state
   :project: OrbbecSdk

.. doxygentypedef:: ob_device_temperature
   :project: OrbbecSdk

.. doxygentypedef:: DEVICE_TEMPERATURE
   :project: OrbbecSdk

.. doxygentypedef:: ob_depth_cropping_mode
   :project: OrbbecSdk

.. doxygentypedef:: OB_DEPTH_CROPPING_MODE
   :project: OrbbecSdk

.. doxygentypedef:: ob_device_type
   :project: OrbbecSdk

.. doxygentypedef:: OB_DEVICE_TYPE
   :project: OrbbecSdk

.. doxygentypedef:: ob_media_type
   :project: OrbbecSdk

.. doxygentypedef:: OB_MEDIA_TYPE
   :project: OrbbecSdk

.. doxygentypedef:: ob_media_state
   :project: OrbbecSdk

.. doxygentypedef:: OB_MEDIA_STATE_EM
   :project: OrbbecSdk

.. doxygentypedef:: ob_depth_precision_level
   :project: OrbbecSdk

.. doxygentypedef:: OB_DEPTH_PRECISION_LEVEL
   :project: OrbbecSdk

.. doxygentypedef:: ob_tof_filter_range
   :project: OrbbecSdk

.. doxygentypedef:: TOF_FILTER_RANGE
   :project: OrbbecSdk

.. doxygentypedef:: ob_point
   :project: OrbbecSdk

.. doxygentypedef:: ob_color_point
   :project: OrbbecSdk

.. doxygentypedef:: ob_sync_mode
   :project: OrbbecSdk

.. doxygentypedef:: OB_SYNC_MODE
   :project: OrbbecSdk

.. doxygentypedef:: ob_tof_exposure_threshold_control
   :project: OrbbecSdk

.. doxygentypedef:: TOF_EXPOSURE_THRESHOLD_CONTROL
   :project: OrbbecSdk

.. doxygentypedef:: ob_multi_device_sync_config
   :project: OrbbecSdk

.. doxygentypedef:: OB_MULTI_DEVICE_SYNC_CONFIG
   :project: OrbbecSdk

.. doxygentypedef:: ob_file_send_callback
   :project: OrbbecSdk

.. doxygentypedef:: ob_device_upgrade_callback
   :project: OrbbecSdk

.. doxygentypedef:: ob_device_state_callback
   :project: OrbbecSdk

.. doxygentypedef:: ob_set_data_callback
   :project: OrbbecSdk

.. doxygentypedef:: ob_get_data_callback
   :project: OrbbecSdk

.. doxygentypedef:: ob_media_state_callback
   :project: OrbbecSdk

.. doxygentypedef:: ob_device_changed_callback
   :project: OrbbecSdk

.. doxygentypedef:: ob_frame_callback
   :project: OrbbecSdk

.. doxygentypedef:: ob_frameset_callback
   :project: OrbbecSdk

.. doxygentypedef:: ob_property_id
   :project: OrbbecSdk

.. doxygentypedef:: OBPropertyType
   :project: OrbbecSdk

.. doxygentypedef:: ob_property_type
   :project: OrbbecSdk

.. doxygentypedef:: OBPropertyItem
   :project: OrbbecSdk

.. doxygentypedef:: ob_property_item
   :project: OrbbecSdk

函数
--------------------------------------------
.. doxygenfunction:: ob_create_context
   :project: OrbbecSdk

.. doxygenfunction:: ob_create_context_with_config
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_context
   :project: OrbbecSdk

.. doxygenfunction:: ob_query_device_list
   :project: OrbbecSdk

.. doxygenfunction:: ob_set_device_changed_callback
   :project: OrbbecSdk

.. doxygenfunction:: ob_enable_device_clock_sync
   :project: OrbbecSdk

.. doxygenfunction:: ob_set_logger_severity
   :project: OrbbecSdk

.. doxygenfunction:: ob_set_logger_to_file
   :project: OrbbecSdk

.. doxygenfunction:: ob_set_logger_to_console
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_list_device_count
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_list_get_device_name
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_list_get_device_pid
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_list_get_device_vid
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_list_get_device_uid
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_list_get_device_serial_number
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_list_get_device
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_list_get_device_by_serial_number
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_list_get_device_by_uid
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_device
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_device_info
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_device_list
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_device_info
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_sensor_list
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_sensor
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_set_int_property
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_int_property
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_set_float_property
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_float_property
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_set_bool_property
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_bool_property
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_set_structured_data
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_structured_data
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_set_raw_data
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_raw_data
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_supported_property_count
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_supported_property
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_is_property_supported
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_int_property_range
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_float_property_range
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_bool_property_range
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_write_ahb
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_read_ahb
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_write_i2c
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_read_i2c
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_write_flash
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_read_flash
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_sync_device_time
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_upgrade
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_device_state
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_state_changed
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_send_file_to_destination
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_activate_authorization
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_write_authorization_code
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_get_calibration_camera_param_list
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_reboot
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_name
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_pid
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_vid
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_uid
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_serial_number
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_firmware_version
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_usb_type
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_hardware_version
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_supported_min_sdk_version
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_asicName
   :project: OrbbecSdk

.. doxygenfunction:: ob_device_info_device_type
   :project: OrbbecSdk

.. doxygenfunction:: ob_camera_param_list_count
   :project: OrbbecSdk

.. doxygenfunction:: ob_camera_param_list_get_param
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_camera_param_list
   :project: OrbbecSdk

.. doxygenfunction:: ob_error_status
   :project: OrbbecSdk

.. doxygenfunction:: ob_error_message
   :project: OrbbecSdk

.. doxygenfunction:: ob_error_function
   :project: OrbbecSdk

.. doxygenfunction:: ob_error_args
   :project: OrbbecSdk

.. doxygenfunction:: ob_error_exception_type
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_error
   :project: OrbbecSdk

.. doxygenfunction:: ob_create_pointcloud_filter
   :project: OrbbecSdk

.. doxygenfunction:: ob_pointcloud_filter_set_camera_param
   :project: OrbbecSdk

.. doxygenfunction:: ob_pointcloud_filter_set_point_format
   :project: OrbbecSdk

.. doxygenfunction:: ob_pointcloud_filter_set_frame_align_state
   :project: OrbbecSdk

.. doxygenfunction:: ob_create_format_convert_filter
   :project: OrbbecSdk

.. doxygenfunction:: ob_format_convert_filter_set_format
   :project: OrbbecSdk

.. doxygenfunction:: ob_filter_reset
   :project: OrbbecSdk

.. doxygenfunction:: ob_filter_process
   :project: OrbbecSdk

.. doxygenfunction:: ob_filter_set_callback
   :project: OrbbecSdk

.. doxygenfunction:: ob_filter_push_frame
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_filter
   :project: OrbbecSdk

.. doxygenfunction:: ob_frame_index
   :project: OrbbecSdk

.. doxygenfunction:: ob_frame_format
   :project: OrbbecSdk

.. doxygenfunction:: ob_frame_get_type
   :project: OrbbecSdk

.. doxygenfunction:: ob_frame_time_stamp
   :project: OrbbecSdk

.. doxygenfunction:: ob_frame_time_stamp_us
   :project: OrbbecSdk

.. doxygenfunction:: ob_frame_system_time_stamp
   :project: OrbbecSdk

.. doxygenfunction:: ob_frame_data
   :project: OrbbecSdk

.. doxygenfunction:: ob_frame_data_size
   :project: OrbbecSdk

.. doxygenfunction:: ob_video_frame_width
   :project: OrbbecSdk

.. doxygenfunction:: ob_video_frame_height
   :project: OrbbecSdk

.. doxygenfunction:: ob_video_frame_metadata
   :project: OrbbecSdk

.. doxygenfunction:: ob_video_frame_metadata_size
   :project: OrbbecSdk

.. doxygenfunction:: ob_video_frame_pixel_available_bit_size
   :project: OrbbecSdk

.. doxygenfunction:: ob_depth_frame_get_value_scale
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_frame
   :project: OrbbecSdk

.. doxygenfunction:: ob_frameset_frame_count
   :project: OrbbecSdk

.. doxygenfunction:: ob_frameset_depth_frame
   :project: OrbbecSdk

.. doxygenfunction:: ob_frameset_color_frame
   :project: OrbbecSdk

.. doxygenfunction:: ob_frameset_ir_frame
   :project: OrbbecSdk

.. doxygenfunction:: ob_frameset_points_frame
   :project: OrbbecSdk

.. doxygenfunction:: ob_accel_frame_value
   :project: OrbbecSdk

.. doxygenfunction:: ob_accel_frame_temperature
   :project: OrbbecSdk

.. doxygenfunction:: ob_gyro_frame_value
   :project: OrbbecSdk

.. doxygenfunction:: ob_gyro_frame_temperature
   :project: OrbbecSdk

.. doxygenfunction:: ob_create_pipeline
   :project: OrbbecSdk

.. doxygenfunction:: ob_create_pipeline_with_device
   :project: OrbbecSdk

.. doxygenfunction:: ob_create_pipeline_with_playback_file
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_pipeline
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_start
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_start_with_config
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_start_with_callback
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_stop
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_get_config
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_wait_for_frameset
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_get_device
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_get_playback
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_get_stream_profile_list
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_enable_frame_sync
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_disable_frame_sync
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_switch_config
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_get_camera_param
   :project: OrbbecSdk

.. doxygenfunction:: ob_get_d2c_depth_profile_list
   :project: OrbbecSdk

.. doxygenfunction:: ob_get_d2c_valid_area
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_start_record
   :project: OrbbecSdk

.. doxygenfunction:: ob_pipeline_stop_record
   :project: OrbbecSdk

.. doxygenfunction:: ob_create_config
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_config
   :project: OrbbecSdk

.. doxygenfunction:: ob_config_enable_stream
   :project: OrbbecSdk

.. doxygenfunction:: ob_config_enable_all_stream
   :project: OrbbecSdk

.. doxygenfunction:: ob_config_disable_stream
   :project: OrbbecSdk

.. doxygenfunction:: ob_config_disable_all_stream
   :project: OrbbecSdk

.. doxygenfunction:: ob_config_set_align_mode
   :project: OrbbecSdk

.. doxygenfunction:: ob_create_recorder
   :project: OrbbecSdk

.. doxygenfunction:: ob_create_recorder_with_device
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_recorder
   :project: OrbbecSdk

.. doxygenfunction:: ob_recorder_start
   :project: OrbbecSdk

.. doxygenfunction:: ob_recorder_stop
   :project: OrbbecSdk

.. doxygenfunction:: ob_recorder_write_frame
   :project: OrbbecSdk

.. doxygenfunction:: ob_create_playback
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_playback
   :project: OrbbecSdk

.. doxygenfunction:: ob_playback_start
   :project: OrbbecSdk

.. doxygenfunction:: ob_playback_stop
   :project: OrbbecSdk

.. doxygenfunction:: ob_set_playback_state_callback
   :project: OrbbecSdk

.. doxygenfunction:: ob_playback_get_device_info
   :project: OrbbecSdk

.. doxygenfunction:: ob_playback_get_camera_param
   :project: OrbbecSdk

.. doxygenfunction:: ob_sensor_get_type
   :project: OrbbecSdk

.. doxygenfunction:: ob_sensor_get_stream_profile_list
   :project: OrbbecSdk

.. doxygenfunction:: ob_sensor_start
   :project: OrbbecSdk

.. doxygenfunction:: ob_sensor_stop
   :project: OrbbecSdk

.. doxygenfunction:: ob_sensor_switch_profile
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_sensor_list
   :project: OrbbecSdk

.. doxygenfunction:: ob_sensor_list_get_sensor_count
   :project: OrbbecSdk

.. doxygenfunction:: ob_sensor_list_get_sensor_type
   :project: OrbbecSdk

.. doxygenfunction:: ob_sensor_list_get_sensor_by_type
   :project: OrbbecSdk

.. doxygenfunction:: ob_sensor_list_get_sensor
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_sensor
   :project: OrbbecSdk

.. doxygenfunction:: ob_stream_profile_format
   :project: OrbbecSdk

.. doxygenfunction:: ob_stream_profile_type
   :project: OrbbecSdk

.. doxygenfunction:: ob_video_stream_profile_fps
   :project: OrbbecSdk

.. doxygenfunction:: ob_video_stream_profile_width
   :project: OrbbecSdk

.. doxygenfunction:: ob_video_stream_profile_height
   :project: OrbbecSdk

.. doxygenfunction:: ob_accel_stream_profile_full_scale_range
   :project: OrbbecSdk

.. doxygenfunction:: ob_accel_stream_profile_sample_rate
   :project: OrbbecSdk

.. doxygenfunction:: ob_gyro_stream_profile_full_scale_range
   :project: OrbbecSdk

.. doxygenfunction:: ob_gyro_stream_profile_sample_rate
   :project: OrbbecSdk

.. doxygenfunction:: ob_stream_profile_list_get_video_stream_profile
   :project: OrbbecSdk

.. doxygenfunction:: ob_stream_profile_list_get_profile
   :project: OrbbecSdk

.. doxygenfunction:: ob_stream_profile_list_count
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_stream_profile_list
   :project: OrbbecSdk

.. doxygenfunction:: ob_delete_stream_profile
   :project: OrbbecSdk

.. doxygenfunction:: ob_get_version
   :project: OrbbecSdk

.. doxygenfunction:: ob_get_major_version
   :project: OrbbecSdk

.. doxygenfunction:: ob_get_minor_version
   :project: OrbbecSdk

.. doxygenfunction:: ob_get_patch_version
   :project: OrbbecSdk

