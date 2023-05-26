/**
 *
 * @file RecordPlayback.hpp
 * @brief For recording and playback functions
 *
 */
#pragma once

#include "Types.hpp"
#include <memory>

struct RecorderImpl;
struct PlaybackImpl;

namespace ob {
class Device;
class Frame;
class DeviceInfo;

using PlaybackCallback = std::function<void(std::shared_ptr<Frame> frame)>;
using MediaStateCallback = std::function<void(OBMediaState state)>;

class OB_EXTENSION_API Recorder {
private:
    std::unique_ptr<RecorderImpl> impl_;

public:
    /**
     * @brief Create a recorder for data recording
     *
     */
    Recorder();
    Recorder(std::unique_ptr<RecorderImpl> impl);

    /**
     * @brief Create a recorder for data recording
     * @param device Devices that need to record device information
     *
     */
    Recorder(std::shared_ptr<Device> device);

    virtual ~Recorder() noexcept;

    /**
     * @brief Enable the recorder, throw an exception after failure
     *
     * @param filename Recorded file name
     * @param async    Whether to execute asynchronously
     *
     */
    void start(const char *filename, bool async = false);

    /**
     * @brief Stop the recorder, throw an exception on failure
     *
     */
    void stop();

    /**
     * @brief Write frame data to the recorder
     *
     * @param frame Write frame data
     *
     */
    void write(std::shared_ptr<Frame> frame);
};

class OB_EXTENSION_API Playback {
private:
    std::unique_ptr<PlaybackImpl> impl_;

public:
    /**
     * @brief Create playback object
     * @param filename Playback filename
     *
     */
    Playback(const char *filename);
    Playback(std::unique_ptr<PlaybackImpl> impl);
    virtual ~Playback() noexcept;

    /**
     * @brief Start playback, the playback data is returned from the callback, and an exception is thrown after failure
     * @param filename Playback filename
     * @param callback Callback for playback data
     * @param type     	Type of playback data
     *
     */
    void start(PlaybackCallback callback, OBMediaType type = OB_MEDIA_ALL);

    /**
     * @brief Stop playback and throw an exception if it fails
     *
     */
    void stop();

    /**
     * @brief Set playback state
     * @param state playback status callback
     *
     */
    void setPlaybackStateCallback(MediaStateCallback state);

    /**
     * @brief Get the device information in the recording file
     *
     * @return DeviceInfo returns device information
     *
     */
    std::shared_ptr<DeviceInfo> getDeviceInfo();

    /**
     * @brief Get the intrinsic and extrinsic parameter information in the recording file
     *
     * @return OBCameraParam returns internal and external parameter information
     *
     */
    OBCameraParam getCameraParam();
};

}  // namespace ob
