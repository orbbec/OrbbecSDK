/**
 * @file Context.hpp
 * @brief The SDK context class, the entry to the underlying SDK, is used to query device lists，handle device callbacks，set the level of the log
 *
 */
#pragma once

#include "Types.hpp"

#include <functional>
#include <memory>

struct ContextImpl;

namespace ob {
class Device;
class DeviceInfo;
class DeviceList;

class OB_EXTENSION_API Context {
private:
    std::unique_ptr<ContextImpl> impl_;

public:
    /**
     * @brief Context is a management class that describes the runtime of the SDK. It is responsible for the applying and releasing of resources for the SDK.
     * The context has the ability to manage multiple devices, is responsible for enumerating devices, monitoring device callbacks, and enabling functions such
     * as multi-device synchronization.
     */
    Context(const char *configPath = "");
    virtual ~Context() noexcept;

    /**
     * @brief Query enumerated device list
     *
     * @return std::shared_ptr<DeviceList> returns a pointer to the device list class
     *
     */
    std::shared_ptr<DeviceList> queryDeviceList();

    /**
     * @brief Create a network device object
     *
     * @param address  ip address
     * @param port port
     * @return std::shared_ptr<Device> Returns the created device object
     *
     */
    std::shared_ptr<Device> createNetDevice(const char *address, uint16_t port);

    using DeviceChangedCallback = std::function<void(std::shared_ptr<DeviceList> removedList, std::shared_ptr<DeviceList> addedList)>;
    /**
     * @brief Set device plug-in callback function
     *
     * @param callback function triggered when the device is plugged and unplugged
     *
     */
    void setDeviceChangedCallback(DeviceChangedCallback callback);

    /**
     * @brief Activate the multi-device synchronization function to synchronize the clock of the created device(the device needs support this function)
     *
     * @param repeatInterval  synchronization time interval (unit: ms; if repeatInterval=0, it means that it will only be synchronized once and will not be
     * executed regularly)
     *
     */
    void enableMultiDeviceSync(uint64_t repeatInterval);

    /**
     * @brief free idle memory from internal frame memory pool
     *
     */
    void freeIdleMemory();

    /**
     * @brief Set the level of the global log will affect both the log level output to the terminal and output to the file
     *
     * @param severity log output level
     *
     */
    static void setLoggerSeverity(OBLogSeverity severity);

    /**
     * @brief Set log output to file
     *
     * @param severity log level output to file
     * @param directory The log file output path. If the path is empty, the existing settings will continue to be used (if the existing configuration is also
     * empty, the log will not be output to the file)
     */
    static void setLoggerToFile(OBLogSeverity severity, const char *directory);

    /**
     * @brief Set log output to terminal
     *
     * @param severity 	log level output to the terminal
     *
     */
    static void setLoggerToConsole(OBLogSeverity severity);

    /**
     * @brief Load license file
     *
     * @param filePath license file path
     * @param key decrypt key
     *
     */
    static void loadLicense(const char *filePath, const char *key = OB_DEFAULT_DECRYPT_KEY);

    /**
     * @brief load license from data
     *
     * @param data license data
     * @param dataLen license data len
     * @param key decrypt key
     *
     */
    static void loadLicenseFromData(const char *data, uint32_t dataLen, const char *key = OB_DEFAULT_DECRYPT_KEY);
};
}  // namespace ob
