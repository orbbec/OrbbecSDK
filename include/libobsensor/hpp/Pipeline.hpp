/**
 * @file Pipeline.hpp
 * @brief The SDK's advanced API type can quickly implement switching streaming, frame synchronization, software filtering, and generating point cloud
 * operations.
 */
#pragma once

#include "Types.hpp"

#include <functional>
#include <memory>

struct PipelineImpl;
struct ConfigImpl;

namespace ob {
class FrameSet;
class Frame;
class Device;
class Playback;
class DeviceInfo;
class Config;
class StreamProfile;
class StreamProfileList;

typedef std::function<void(std::shared_ptr<FrameSet> frame)> FrameSetCallback;

class OB_EXTENSION_API Pipeline {
private:
    std::unique_ptr<PipelineImpl> impl_;

public:
    /**
     * @brief Pipeline is a high-level interface for applications, algorithms related RGBD data streams. Pipeline can provide alignment inside and synchronized
     * FrameSet. Pipeline() no parameter version, which opens the first device in the list of devices connected to the OS by default. If the application has
     * obtained the device through the DeviceList, opening the Pipeline() at this time will throw an exception that the device has been created.
     */
    Pipeline();
    /**
     * @brief
     * Pipeline(std::shared_ptr< Device > device ) Function for multi-device operations. Multiple devices need to be obtained through DeviceList, and the device
     * and pipeline are bound through this interface.
     */
    Pipeline(std::shared_ptr<Device> device);

    /**
     * @brief Create a pipeline for playback of recorded stream files
     *
     * @param filename Playback file path
     *
     */
    Pipeline(const char *filename);
    ~Pipeline() noexcept;

    /**
     * @brief Start the pipeline with configuration parameters
     *
     * @param config Parameter configuration of pipeline
     *
     */
    void start(std::shared_ptr<Config> config);

    /**
     * @brief Start the pipeline and set the frameset data callback
     *
     * @param config parameter configuration of pipeline
     * @param callback  Set the callback to be triggered when all frame data in the frameset arrives
     *
     */
    void start(std::shared_ptr<Config> config, FrameSetCallback callback);

    /**
     * @brief Stop pipeline
     *
     */
    void stop();

    /**
     * @brief Get pipeline configuration parameters
     *
     * @return std::shared_ptr<Config> returns the configured parameters
     *
     */
    std::shared_ptr<Config> getConfig();

    /**
     * @brief Waiting for frameset data
     *
     * @param timeout_ms  Waiting timeout (ms)
     * @return std::shared_ptr<FrameSet> returns the waiting frameset data
     *
     */
    std::shared_ptr<FrameSet> waitForFrames(uint32_t timeout_ms);

    /**
     * @brief Get device object
     *
     * @return std::shared_ptr<Device> returns the device object
     *
     */
    std::shared_ptr<Device> getDevice();

    /**
     * @brief Get playback object
     *
     * @return std::shared_ptr<Playback> returns the playback object
     *
     */
    std::shared_ptr<Playback> getPlayback();

    /**
     * @brief Get the stream profile of specified sensor
     *
     * @param sensorType Type of sensor

     * @return std::shared_ptr<StreamProfileList> returns the stream profile list
     *
     */
    std::shared_ptr<StreamProfileList> getStreamProfileList(OBSensorType sensorType);

    /**
     * @brief Turn on frame synchronization
     *
     */
    void enableFrameSync();

    /**
     * @brief Turn off frame synchronization
     *
     */
    void disableFrameSync();

    /**
     * @brief Get camera parameters
     * @attention If D2C is enabled, it will return the camera parameters after D2C, if not, it will return to the default parameters
     *
     * @return  OBCameraParam returns camera parameters
     *
     */
    OBCameraParam getCameraParam();

    /**
     * @brief Return a list of D2C-enabled depth sensor resolutions corresponding to the input color sensor resolution
     * @param colorProfile Input color sensor resolution
     * @param alignMode Input align mode
     *
     * @return std::shared_ptr<StreamProfileList> returns a list of depth sensor resolutions
     *
     */
    std::shared_ptr<StreamProfileList> getD2CDepthProfileList(std::shared_ptr<StreamProfile> colorProfile, OBAlignMode alignMode);

    /**
     * @brief Get valid area between minimum distance and maximum distance after D2C
     *
     * @param minimumDistance minimum working distance
     * @param maximumDistance maximum working distance
     * @return OBRect returns the area information valid after D2C at the working distance
     *
     */
    OBRect getD2CValidArea(uint32_t minimumDistance, uint32_t maximumDistance = 0);

    /**
     * @brief Dynamically switch the corresponding config configuration
     *
     * @param config Updated config configuration
     *
     */
    void switchConfig(std::shared_ptr<Config> config);

    /**
     * @brief start recording
     *
     * @param filename Record file name
     *
     */
    void startRecord(const char *filename);

    /**
     * @brief Stop recording
     *
     */
    void stopRecord();
};

class OB_EXTENSION_API Config {
private:
    std::unique_ptr<ConfigImpl> impl_;

public:
    Config();
    ~Config() noexcept;

    /**
     * @brief Configure the stream to be enabled
     *
     * @param streamProfile Stream  configuration
     *
     */
    void enableStream(std::shared_ptr<StreamProfile> streamProfile);
    /**
     * @brief Configure all streams to be enabled
     *
     */
    void enableAllStream();
    /**
     * @brief Configure the stream to be disabled
     *
     * @param streamType Stream configuration
     *
     */
    void disableStream(OBStreamType streamType);
    /**
     * @brief Configure all streams to be disabled
     *
     */
    void disableAllStream();

    /**
     * @brief Set the alignment mode
     *
     * @param mode Align State Mode
     *
     */
    void setAlignMode(OBAlignMode mode);

    /**
     * @brief Whether the depth needs to be scaled after setting D2C
     *
     * @param enable Whether scaling is required
     *
     */
    void setDepthScaleRequire(bool enable);

    /**
     * @brief Set the D2C target resolution, which is applicable to cases where the color stream is not enabled using the OrbbecSDK and the depth needs to be
     * D2C Note: When you use OrbbecSDK to enable the color stream, you also use this interface to set the D2C target resolution. The configuration of the
     * enabled Color stream is preferred for D2C.
     * @param d2cTargetWidth  The D2C target has a wide resolution
     * @param d2cTargetHeight The D2C target has a high resolutio
     *
     */
    void setD2CTargetResolution(uint32_t d2cTargetWidth, uint32_t d2cTargetHeight);

    friend class Pipeline;
};

}  // namespace ob
