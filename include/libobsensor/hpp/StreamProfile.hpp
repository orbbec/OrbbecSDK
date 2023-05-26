/**
 * @file StreamProfile.hpp
 * @brief The stream profile related type is used to get information such as the width, height, frame rate, and format of the stream.
 *
 */
#pragma once

#include "Types.hpp"

#include <iostream>
#include <memory>

struct StreamProfileImpl;
struct StreamProfileListImpl;

namespace ob {

class VideoStreamProfile;
class GyroStreamProfile;
class AccelStreamProfile;
class Config;

class OB_EXTENSION_API StreamProfile : public std::enable_shared_from_this<StreamProfile> {
protected:
    std::unique_ptr<StreamProfileImpl> impl_;

public:
    StreamProfile(std::unique_ptr<StreamProfileImpl> impl);
    StreamProfile(StreamProfile &streamProfile);
    ~StreamProfile() noexcept;

    /**
     * @brief Get the format of the stream
     *
     * @return OBFormat returns the format of the stream
     *
     */
    OBFormat format() const;
    /**
     * @brief Get the type of stream
     *
     * @return OBStreamType returns the type of the stream
     *
     */
    OBStreamType type() const;

    /**
     * @brief Check if frame object is compatible with the given type
     *
     * @tparam T  Given type
     * @return bool return result
     *
     */
    template <typename T> bool is();

    /**
     * @brief Object type conversion
     *
     * @tparam T  Target type
     * @return std::shared_ptr<T>  returns the result, if it cannot be converted, an exception will be thrown
     *
     */
    template <typename T> std::shared_ptr<T> as() {
        if(!is<T>())
            throw "unsupported operation, object's type is not require type";

        return std::static_pointer_cast<T>(std::const_pointer_cast<StreamProfile>(shared_from_this()));
    }

    friend class Sensor;
    friend class Config;
    friend class Pipeline;
};

class OB_EXTENSION_API VideoStreamProfile : public StreamProfile {
public:
    VideoStreamProfile(StreamProfile &profile);
    ~VideoStreamProfile() noexcept;

    /**
     * @brief Get stream frame rate
     *
     * @return uint32_t returns the frame rate of the stream
     *
     */
    uint32_t fps() const;
    /**
     * @brief Get stream width
     *
     * @return uint32_t returns the width of the stream
     *
     */
    uint32_t width() const;
    /**
     * @brief Get stream height
     *
     * @return uint32_t returns the high of the stream
     *
     */
    uint32_t height() const;
};

class OB_EXTENSION_API AccelStreamProfile : public StreamProfile {
public:
    AccelStreamProfile(StreamProfile &profile);
    ~AccelStreamProfile() noexcept;

    /**
     * @brief Get full scale range
     *
     * @return OBAccelFullScaleRange  returns the scale range value
     *
     */
    OBAccelFullScaleRange fullScaleRange() const;

    /**
     * @brief Get sampling frequency
     *
     * @return OBAccelFullScaleRange  returns the sampling frequency
     *
     */
    OBAccelSampleRate sampleRate() const;
};

class OB_EXTENSION_API GyroStreamProfile : public StreamProfile {
public:
    GyroStreamProfile(StreamProfile &profile);
    ~GyroStreamProfile() noexcept;

    /**
     * @brief Get full scale range
     *
     * @return OBAccelFullScaleRange  returns the scale range value
     *
     */
    OBGyroFullScaleRange fullScaleRange() const;

    /**
     * @brief Get sampling frequency
     *
     * @return OBAccelFullScaleRange  returns the sampling frequency
     *
     */
    OBGyroSampleRate sampleRate() const;
};

template <typename T> bool StreamProfile::is() {
    switch(this->type()) {
    case OB_STREAM_VIDEO:
    case OB_STREAM_IR:
    case OB_STREAM_IR_LEFT:
    case OB_STREAM_IR_RIGHT:
    case OB_STREAM_COLOR:
    case OB_STREAM_DEPTH:
        return typeid(T) == typeid(VideoStreamProfile);
    case OB_STREAM_ACCEL:
        return typeid(T) == typeid(AccelStreamProfile);
    case OB_STREAM_GYRO:
        return typeid(T) == typeid(GyroStreamProfile);
    default:
        break;
    }
    return false;
}

class OB_EXTENSION_API StreamProfileList {
protected:
    std::unique_ptr<StreamProfileListImpl> impl_;

public:
    StreamProfileList(std::unique_ptr<StreamProfileListImpl> impl);
    ~StreamProfileList() noexcept;

    /**
     * @brief Get stream profile count
     *
     * @return uint32_t returns the number of StreamProfile
     *
     */
    uint32_t count() const;

    /**
     * @brief Get StreamProfile by index number
     *
     * @param index Device index to be created，the range is [0, count-1],if the index exceeds the range, an exception will be thrown
     * @return std::shared_ptr<StreamProfile> returns StreamProfile object
     *
     */
    const std::shared_ptr<StreamProfile> getProfile(uint32_t index);

    /**
     * @brief Match the corresponding stream profile through the passed in parameters. If there are multiple matches,
     * the first one in the list will be returned by default. If no matched profile found, will throw exception!
     *
     * @param width Width. If no matching condition is required, it can be passed to 0
     * @param height Height. If no matching condition is required, it can be passed to 0
     * @param format Type. If no matching condition is required, it can be passed to OB_FORMAT_UNKNOWN
     * @param fps Frame rate. If no matching condition is required, it can be passed to 0
     * @return std::shared_ptr<StreamProfile> Returns the matching resolution
     *
     */
    const std::shared_ptr<VideoStreamProfile> getVideoStreamProfile(int width = 0, int height = 0, OBFormat format = OB_FORMAT_UNKNOWN, int fps = 0);
};

}  // namespace ob
