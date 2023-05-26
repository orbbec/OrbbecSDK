/**
 * @file Sensor.hpp
 * @brief Sensor related types, used to obtain stream configuration, open and close streams, set and get sensor properties.
 *
 */
#pragma once

#include "Types.hpp"

#include <functional>
#include <memory>

struct SensorImpl;
struct SensorListImpl;

namespace ob {
class StreamProfile;
class StreamProfileList;
class Device;
class Frame;
class ImuFrame;

/**
 * @brief frame data callback
 *
 * @param frame  frame data
 *
 */
using FrameCallback = std::function<void(std::shared_ptr<Frame> frame)>;

class OB_EXTENSION_API Sensor {
protected:
    std::unique_ptr<SensorImpl> impl_;

public:
    Sensor(std::unique_ptr<SensorImpl> impl);
    virtual ~Sensor() noexcept;

    /**
     * @brief sensor type
     *
     * @return OBSensorType returns the sensor type
     *
     */
    OBSensorType type();

    /**
     * @brief Get the list of stream profiles
     *
     * @return std::shared_ptr<StreamProfileList> returns the stream profile list
     *
     */
    const std::shared_ptr<StreamProfileList> getStreamProfileList();

    /**
     * @brief Open frame data stream and set up a callback
     *
     * @param streamProfile Stream configuration
     * @param callback Set the callback when frame data arrives
     *
     */
    void start(std::shared_ptr<StreamProfile> streamProfile, FrameCallback callback);
    /**
     * @brief Stop stream
     *
     */
    void stop();

    /**
     * @brief Dynamically switch resolutions
     *
     * @param streamProfile Resolution to switch
     *
     */
    void switchProfile(std::shared_ptr<StreamProfile> streamProfile);
};

class OB_EXTENSION_API SensorList {
private:
    std::unique_ptr<SensorListImpl> impl_;

public:
    SensorList(std::unique_ptr<SensorListImpl> impl);

    virtual ~SensorList() noexcept;

    /**
     * @brief Get sensor count
     *
     * @return uint32_t returns the number of Sensors
     *
     */
    uint32_t count();

    /**
     * @brief Get the type of the specified Sensor
     *
     * @param index  Sensor index
     * @return OBSensorType returns the Sensor type
     *
     */
    OBSensorType type(uint32_t index);

    /**
     * @brief Get Sensor by index number
     *
     * @param index  To create a device cable, the range is [0, count-1], if the index exceeds the range, an exception will be thrown

     * @return std::shared_ptr<Sensor> returns the Sensor object
     *
     */
    std::shared_ptr<Sensor> getSensor(uint32_t index);

    /**
     * @brief Obtain the Sensor through the Sensor type
     *
     * @param sensorType Sensor type to be obtained
     * @return std::shared_ptr<Sensor>  returns a Sensor object, if the specified type of Sensor does not exist, it will return empty
     *
     */
    std::shared_ptr<Sensor> getSensor(OBSensorType sensorType);
};
}  // namespace ob
