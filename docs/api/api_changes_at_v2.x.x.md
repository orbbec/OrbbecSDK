# API Changes at v2.x.x

This document outlines the key differences in APIs between OrbbecSDK v2.x and the previous version [OrbbecSDK v1.x](https://github.com/orbbec/OrbbecSDK). The primary areas of change are concentrated within the **device** and **pipeline** modules. We have deprecated or modified certain C and C++ interfaces to accommodate internal enhancements, improved functionality, and to reflect the discontinuation of support for certain devices.

## Device Module

The updates to the **device** module in our APIs primarily involve adjustments to interfaces that were either reserved for internal use or were not commonly utilized by developers.

### Removed C Interfaces
- ob_device_set_structured_data_ext
- ob_device_get_structured_data_ext
- ob_device_set_raw_data
- ob_device_get_raw_data
- ob_device_get_protocol_version
- ob_device_get_cmd_version
- ob_device_write_ahb
- ob_device_read_ahb
- ob_device_write_i2c
- ob_device_read_i2c
- ob_device_write_flash
- ob_device_read_flash
- ob_device_set_sync_config
- ob_device_info_usb_type
- ob_device_send_file_to_destination
- ob_device_activate_authorization
- ob_device_write_authorization_code
- ob_device_load_depth_filter_config
- ob_device_reset_default_depth_filter_config

### Removed C++ Interfaces
- writeAHB
- readAHB
- writeI2C
- readI2C
- writeFlash
- readFlash
- writeCustomerData
- readCustomerData
- setRawData
- getRawData
- setStructuredDataExt
- getStructuredDataExt
- getProtocolVersion
- getCmdVersion
- sendFile
- activateAuthorization
- loadLicense
- loadLicenseFromData
- getSyncConfig
- setSyncConfig
- sendFile
- activateAuthorization
- writeAuthorizationCode
- loadDepthFilterConfig
- resetDefaultDepthFilterConfig

## Pipeline Module

The updates to the **pipeline** module in our APIs are primarily focused on the recording and playback functionality, which will be redesigned in upcoming releases. Additionally, other updates to the **pipeline** module are targeted at APIs that were either reserved for internal use or were not commonly utilized by developers.

### Removed C Interfaces
- ob_create_pipeline_with_playback_file
- ob_pipeline_get_playback
- ob_pipeline_start_record
- ob_pipeline_stop_record
- ob_get_d2c_valid_area
- ob_get_d2c_range_valid_area

### Removed C++ Interfaces
- Pipeline(const char \*filename)
- getPlayback
- startRecord
- stopRecord
- getD2CValidArea
