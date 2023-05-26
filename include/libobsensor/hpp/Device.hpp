/**
 * @file Device.hpp
 * @brief Device related types, including operations such as getting and creating a device, setting and obtaining device attributes, and obtaining sensors
 *
 */
#pragma once
#include "Types.hpp"

#include "libobsensor/h/Property.h"

#include <memory>
#include <string>
#include <vector>

struct DeviceImpl;
struct DeviceInfoImpl;
struct DeviceListImpl;
struct CameraParamListImpl;

namespace ob {
class SensorList;
class Context;
class DeviceInfo;
class Sensor;
class CameraParamList;
class OBDepthWorkModeList;

class OB_EXTENSION_API Device {
private:
    std::unique_ptr<DeviceImpl> impl_;

public:
    /**
     * @brief Describe the entity of the RGBD camera, representing a specific model of RGBD camera
     *
     */
    Device(std::unique_ptr<DeviceImpl> impl);
    virtual ~Device() noexcept;
    /**
     * @brief Get device information
     *
     * @return std::shared_ptr<DeviceInfo> returns device information
     *
     */
    std::shared_ptr<DeviceInfo> getDeviceInfo();

    /**
     * @brief Get device sensor list
     *
     * @return std::shared_ptr<SensorList> returns the sensor list
     *
     */
    std::shared_ptr<SensorList> getSensorList();

    /**
     * @brief Get specific type of sensor
     * if device not open, SDK will automatically open the connected device and return to the instance
     *
     * @return std::shared_ptr<Sensor> eturns the sensor example, if the device does not have the device,returns nullptr
     *
     */
    std::shared_ptr<Sensor> getSensor(OBSensorType type);
    /**
     * @brief Set int type of device property
     *
     * @param propertyId Property id
     * @param property Property to be set
     *
     */
    void setIntProperty(OBPropertyID propertyId, int32_t property);

    /**
     * @brief Set float type of device property
     *
     * @param propertyId Property id
     * @param property Property to be set
     *
     */
    void setFloatProperty(OBPropertyID propertyId, float property);

    /**
     * @brief Set bool type of device property
     *
     * @param propertyId Property id
     * @param property Property to be set
     *
     */
    void setBoolProperty(OBPropertyID propertyId, bool property);

    /**
     * @brief Get int type of device property
     *
     * @param propertyId Property id
     * @return int32_t Property to get
     *
     */
    int32_t getIntProperty(OBPropertyID propertyId);

    /**
     * @brief Get float type of device property
     *
     * @param propertyId Property id
     * @return float Property to get
     *
     */
    float getFloatProperty(OBPropertyID propertyId);

    /**
     * @brief Get bool type of device property
     *
     * @param propertyId Property id
     * @return bool Property to get
     *
     */
    bool getBoolProperty(OBPropertyID propertyId);

    /**
     * @brief Get int type device property range (ncluding current valueand default value)
     *
     * @param propertyId Property id
     * @return OBIntPropertyRange Property range
     *
     */
    OBIntPropertyRange getIntPropertyRange(OBPropertyID propertyId);

    /**
     * @brief Get float type device property range((including current valueand default value)
     *
     * @param propertyId Property id
     * @return OBFloatPropertyRange Property range
     *
     */
    OBFloatPropertyRange getFloatPropertyRange(OBPropertyID propertyId);

    /**
     * @brief Get bool type device property range (including current value anddefault value)
     *
     * @param propertyId Property id
     * @return OBBoolPropertyRange Property range
     *
     */
    OBBoolPropertyRange getBoolPropertyRange(OBPropertyID propertyId);

    /**
     * @brief AHB register write
     *
     * @param reg Register to be written
     * @param mask  The mask to be writen
     * @param value The value to be written
     *
     */
    void writeAHB(uint32_t reg, uint32_t mask, uint32_t value);

    /**
     * @brief AHB AHB register read
     *
     * @param reg Register to be read
     * @param mask The mask to be read
     * @param value The value to be returned
     *
     */
    void readAHB(uint32_t reg, uint32_t mask, uint32_t *value);

    /**
     * @brief I2C register write
     *
     * @param reg I2C module ID to be written
     * @param reg Register to be written
     * @param mask The mask to be written
     * @param value he value to be written
     *
     */
    void writeI2C(uint32_t moduleId, uint32_t reg, uint32_t mask, uint32_t value);

    /**
     * @brief I2C registers read
     *
     * @param reg I2C module ID to be read
     * @param reg Register to be read
     * @param mask The mask to be read
     * @param value The value to be returned
     *
     */
    void readI2C(uint32_t moduleId, uint32_t reg, uint32_t mask, uint32_t *value);

    /**
     * @brief Set the properties of writing to Flash
     *
     * @param offset flash offset address
     * @param data Property data to be written
     * @param dataSize  The size of the property to be written
     * @param callback Write flash progress callback
     * @param async    Whether to execute asynchronously
     *
     */
    void writeFlash(uint32_t offset, const void *data, uint32_t dataSize, SetDataCallback callback, bool async = false);

    /**
     * @brief Read Flash property
     *
     * @param offset flash offset address
     * @param data Property data to be read
     * @param dataSize  The size of the property to get
     * @param callback Read data returned by flash and progress callback
     * @param async    Whether to execute asynchronously
     *
     */
    void readFlash(uint32_t offset, uint32_t dataSize, GetDataCallback callback, bool async = false);

    /**
     * @brief Set raw data type of device property [Asynchronous callback]
     *
     * @param propertyId Property id
     * @param data Property data to be set
     * @param dataSize The size of the property data to be set
     * @param callback rawdata set progress callback
     * @param async    Whether to execute asynchronously
     *
     */
    void setRawData(OBPropertyID propertyId, const void *data, uint32_t dataSize, SetDataCallback callback, bool async = false);

    /**
     * @brief Get raw data type of device property [Asynchronous callback]
     *
     * @param propertyId Property id
     * @param data Property data obtained
     * @param dataSize Get the size of the property
     * @param callback  Get the returned data and progress callback
     * @param async    Whether to execute asynchronously
     *
     */
    void getRawData(OBPropertyID propertyId, GetDataCallback callback, bool async = false);

    /**
     * @brief Set structured data type of device property
     *
     * @param propertyId Property id
     * @param data Property data to be set
     * @param dataSize The size of the attribute to be set
     *
     */
    void setStructuredData(OBPropertyID propertyId, const void *data, uint32_t dataSize);

    /**
     * @brief Get structured data type of device property
     *
     * @param propertyId Property id
     * @param data Property data obtained
     * @param dataSize Get the size of the attribute
     *
     */
    void getStructuredData(OBPropertyID propertyId, void *data, uint32_t *dataSize);

    /**
     * @brief Get structured data type of device property
     *
     * @param propertyId Property id
     * @param dataBundle Target data
     * @param callback Callback of setting
     *
     */
    void setStructuredDataExt(OBPropertyID propertyId, std::shared_ptr<OBDataBundle> dataBundle, SetDataCallback callback);

    /**
     * @brief Get structured data type of device property
     *
     * @param propertyId Property id
     * @return Data bundle
     *
     */
    std::shared_ptr<OBDataBundle> getStructuredDataExt(OBPropertyID propertyId);

    /**
     * @brief Get the property protocol version
     *
     * @return OBProtocolVersion
     *
     */
    OBProtocolVersion getProtocolVersion();

    /**
     * @brief Get cmdVersion of property
     *
     * @param propertyId Property id
     * @return OBCmdVersion
     *
     */
    OBCmdVersion getCmdVersion(OBPropertyID propertyId);

    /**
     * @brief Get number of devices supported property
     *
     * @return uint32_t returns the number of supported attributes
     *
     */
    uint32_t getSupportedPropertyCount();

    /**
     * @brief Get device supported properties
     *
     * @param uint32_t Property index
     * @return OBPropertyItem returns the type of supported properties
     *
     */
    OBPropertyItem getSupportedProperty(uint32_t index);

    /**
     * @brief Judge property permission support
     *
     * @param propertyId Property id
     * @param permission Types of read and write permissions that need to be interpreted
     * @return bool returns whether it is supported
     *
     */
    bool isPropertySupported(OBPropertyID propertyId, OBPermissionType permission);

    /**
     * @brief Synchronize device time (synchronize local systemtime to device)
     * @return uint64_t command (round trip time， rtt）
     *
     */
    uint64_t syncDeviceTime();

    /**
     * @brief Upgrade the device firmware
     *
     * @param filePath Firmware path
     * @param callback  Firmware upgrade progress and status callback
     * @param async    Whether to execute asynchronously
     *
     */
    void deviceUpgrade(const char *filePath, DeviceUpgradeCallback callback, bool async = true);

    /**
     * @brief Send files to the specified path on the device side [Asynchronouscallback]
     *
     * @param filePath Original file path
     * @param dstPath  Accept the save path on the device side
     * @param callback File transfer callback
     * @param async    Whether to execute asynchronously
     *
     */
    void sendFile(const char *filePath, const char *dstPath, SendFileCallback callback, bool async = true);

    /**
     * @brief Get the current state
     * @return OBDeviceState device state information
     *
     */
    OBDeviceState getDeviceState();

    /**
     * @brief Set the device state changed callbacks
     *
     * @param callback The callback function that is triggered when the device status changes (for example, the frame rate is automatically reduced or the
     * stream is closed due to high temperature, etc.)
     */
    void setDeviceStateChangedCallback(DeviceStateChangedCallback callback);

    /**
     * @brief Verify device authorization code
     * @param authCode Authorization code
     * @return bool whether the activation is successfu
     *
     */
    bool activateAuthorization(const char *authCode);

    /**
     * @brief Write authorization code
     * @param authCode  Authorization code
     *
     */
    void writeAuthorizationCode(const char *authCodeStr);

     /**
     * @brief Get the original parameter list of camera calibration saved in the device. The parameters in the list do not correspond to the current
     * open-current configuration. You need to select the parameters according to the actual situation, and may need to do scaling, mirroring and other
     * processing. Non-professional users are recommended to use the Pipeline::getCameraParam() interface.
     *
     * @return std::shared_ptr<CameraParamList> camera parameter list
     *
     */
    std::shared_ptr<CameraParamList> getCalibrationCameraParamList();

    /**
     * @brief Get current depth work mode
     *
     * @return ob_depth_work_mode Current depth work mode
     *
     */
    OBDepthWorkMode getCurrentDepthWorkMode();

    /**
     * @brief Switch depth work mode by OBDepthWorkMode. Prefer invoke switchDepthWorkMode(const char *modeName) to switch depth mode
     *        when known the complete name of depth work mode.
     * @param[in] workMode Depth work mode come from ob_depth_work_mode_list which return by ob_device_get_depth_work_mode_list
     *
     */
    OBStatus switchDepthWorkMode(const OBDepthWorkMode &workMode);

    /**
     * @brief Switch depth work mode by work mode name.
     *
     * @param[in] modeName Depth work mode name which equals to OBDepthWorkMode.name
     *
     */
    OBStatus switchDepthWorkMode(const char *modeName);

    /**
     * @brief Request support depth work mode list
     * @return OBDepthWorkModeList list of ob_depth_work_mode
     *
     */
    std::shared_ptr<OBDepthWorkModeList> getDepthWorkModeList();

    /**
     * @brief Device restart
     * @attention The device will be disconnected and reconnected. After the device is disconnected, the access to the Device object interface may be abnormal.
     *   Please delete the object directly and obtain it again after the device is reconnected.
     *
     */
    void reboot();

    /**
     * @brief Gets the current device synchronization configuration
     * @brief Device synchronization: including exposure synchronization function and multi-camera synchronization function of different sensors within a single
     * machine
     *
     * @return OBDeviceSyncConfig returns the device synchronization configuration
     *
     */
    OBDeviceSyncConfig getSyncConfig();

    /**
     * @brief Set the device synchronization configuration
     * @brief Used to configure the exposure synchronization function and multi-camera synchronization function of different sensors in a single machine
     *
     * @attention Calling this function will directly write the configuration to the device Flash, and it will still take effect after the device restarts. To
     * avoid affecting the Flash lifespan, do not update the configuration frequently.
     *
     * @param deviceSyncConfig Device synchronization configuration
     *
     */
    void setSyncConfig(const OBDeviceSyncConfig &deviceSyncConfig);

    friend class Pipeline;
    friend class Recorder;
};

/**
 * @brief A class describing device information, representing the name, id, serial number and other basic information of an RGBD camera.
 *
 */
class OB_EXTENSION_API DeviceInfo {
private:
    std::unique_ptr<DeviceInfoImpl> impl_;

public:
    DeviceInfo(std::unique_ptr<DeviceInfoImpl> impl);
    virtual ~DeviceInfo() noexcept;
    /**
     * @brief Get device name
     *
     * @return const char * returns the device name
     *
     */
    const char *name();

    /**
     * @brief Get the pid of the device
     *
     * @return int returns the pid of the device
     *
     */
    int pid();

    /**
     * @brief Get the vid of the device
     *
     * @return int returns the vid of the device
     *
     */
    int vid();

    /**
     * @brief Get system assigned uid for distinguishing between different devices
     *
     * @return const char * returns the uid of the device
     *
     */
    const char *uid();

    /**
     * @brief Get the serial number of the device
     *
     * @return const char * returns the serial number of the device
     *
     */
    const char *serialNumber();

    /**
     * @brief Get the version number of the firmware
     *
     * @return const char* returns the version number of the firmware
     *
     */
    const char *firmwareVersion();

    /**
     * @brief Get usb connection type (DEPRECATED)
     *
     * @return const char* returns usb connection type
     *
     */
    DEPRECATED const char *usbType();

    /**
     * @brief Get device connection type
     *
     * @return const char* returns connection type
     *
     */
    const char *connectionType();

    /**
     * @brief Get the version number of the hardware
     *
     * @return const char* returns the version number of the hardware
     *
     */
    const char *hardwareVersion();

    /**
     * @brief Get the minimum version number of the SDK supported by the device
     *
     * @return const char* returns the minimum SDK version number supported by the device
     *
     */
    const char *supportedMinSdkVersion();

    /**
     * @brief Get chip type name
     *
     * @return const char* returns the chip type name
     *
     */
    const char *asicName();

    /**
     * @brief Get device type
     *
     * @return OBDeviceType returns the device type
     *
     */
    OBDeviceType deviceType();

    friend class Context;
    friend class DeviceList;
    friend class Pipeline;
};

class OB_EXTENSION_API DeviceList {
private:
    std::unique_ptr<DeviceListImpl> impl_;

public:
    DeviceList(std::unique_ptr<DeviceListImpl> impl);
    ~DeviceList() noexcept;

    /**
     * @brief Get device count
     *
     * @return uint32_t returns the number of devices
     *
     */
    uint32_t deviceCount();

    /**
     * @brief Get the name of the specified device (DEPRECATED)
     *
     * @param index Device index
     * @return int returns the name of the device
     *
     */
    DEPRECATED const char *name(uint32_t index);

    /**
     * @brief Get the pid of the specified device
     *
     * @param index Device index
     * @return int returns the pid of the device
     *
     */
    int pid(uint32_t index);

    /**
     * @brief Get the vid of the specified device
     *
     * @param index Device index
     * @return int returns the vid of the device
     *
     */
    int vid(uint32_t index);

    /**
     * @brief Get the uid of the specified device
     *
     * @param index Device index
     * @return const char * returns the uid of the device
     *
     */
    const char *uid(uint32_t index);

    /**
     * @brief Get the serial number of the specified device
     *
     * @param index device index
     * @return const char * returns the serial number of the device
     *
     */
    const char *serialNumber(uint32_t index);

    /**
     * @brief Get the specified device object from the device list
     * @attention If the device has been acquired and created elsewhere, repeated acquisition will throw an exception
     * @param index index of the device to create
     * @return std::shared_ptr<Device> returns the device object
     *
     */
    std::shared_ptr<Device> getDevice(uint32_t index);

    /**
     * @brief Get the specified device object from the device list
     * @attention If the device has been acquired and created elsewhere, repeated acquisition will throw an exception
     * @param serialNumber The serial number of the device to be created
     * @return std::shared_ptr<Device> returns the device object
     *
     */
    std::shared_ptr<Device> getDeviceBySN(const char *serialNumber);

    /**
     * @brief Get the specified device object from the device list
     * @attention If the device has been acquired and created elsewhere, repeated acquisition will throw an exception
     * @param uid If the device has been acquired and created elsewhere, repeated acquisition will throw an exception
     * @return std::shared_ptr<Device> If the device has been acquired and created elsewhere, repeated acquisition will throw an exception
     *
     */
    std::shared_ptr<Device> getDeviceByUid(const char *uid);
};

class OB_EXTENSION_API CameraParamList {
private:
    std::unique_ptr<CameraParamListImpl> impl_;

public:
    CameraParamList(std::unique_ptr<CameraParamListImpl> impl);
    ~CameraParamList() noexcept;
    /**
     * @brief Number of camera parameter groups
     *
     * @return uint32_t  returns the number of camera parameter groups
     *
     */
    uint32_t count();

    /**
     * @brief Get camera parameters
     *
     * @param index parameter index
     * @return OBCameraParam returns the corresponding group parameters
     *
     */
    OBCameraParam getCameraParam(uint32_t index);
};

/**
 * @brief Type contain list of OBDepthWorkMode
 *
 */
class OB_EXTENSION_API OBDepthWorkModeList {
private:
    std::unique_ptr<OBDepthWorkModeListImpl> impl_;

public:
    OBDepthWorkModeList(std::unique_ptr<OBDepthWorkModeListImpl> impl_);
    ~OBDepthWorkModeList();
    /**
     * @brief Get the count of OBDepthWorkMode
     *
     * @return Count of OBDepthWorkMode
     *
     */
    uint32_t count();

     /**
     * @brief Get OBDepthWorkMode at index in OBDepthWorkModeList
     *
     * @param[in] index Target OBDepthWorkMode's index
     *
     * @return OBDepthWorkMode at index
     *
     */
    OBDepthWorkMode getOBDepthWorkMode(uint32_t index);

    /**
     * @brief Get depth work mode name at index
     *
     * @return Depth work mode name
     *
     */
    std::string getName(uint32_t index);

    /**
     * @brief Get OBDepthWorkMode at index in OBDepthWorkModeList
     *
     * @param[in] index Target OBDepthWorkMode's index
     *
     * @return OBDepthWorkMode at index
     *
     */
    OBDepthWorkMode operator[](uint32_t index);
};

}  // namespace ob
