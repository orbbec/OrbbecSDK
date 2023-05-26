/**
 *
 * @file Frame.hpp
 * @brief Frame related type, which is mainly used to obtain frame data and frame information.
 *
 *
 */
#pragma once

#include "Types.hpp"

#include <memory>
#include <iostream>
#include <typeinfo>

/**
 *    Frame hierarchy
 *                 Frame
 *                   |
 *            +------+----------+----------+-----------+
 *            |      |          |         |            |
 *      VideoFrame PointsFrame AccelFrame GyroFrame FrameSet
 *          |
 *       +--+------+---------+
 *      |          |         |
 *  ColorFrame DepthFrame IRFrame
 *                           |
 *                     +-----+-----+
 *                     |           |
 *                IRLeftFrame IRRightFrame
 */

struct FrameImpl;

namespace ob {
class StreamProfile;
class Filter;
class FrameHelper;

typedef std::function<void(void *buffer, void *context)> BufferDestroyCallback;

class OB_EXTENSION_API Frame : public std::enable_shared_from_this<Frame> {
protected:
    std::unique_ptr<FrameImpl> impl_;

public:
    Frame(std::unique_ptr<FrameImpl> impl);
    Frame(Frame &frame);

    virtual ~Frame() noexcept;
    /**
     *
     * @brief Get the type of frame
     *
     * @return OBFrameType returns the type of frame
     *
     */
    virtual OBFrameType type();

    /**
     *
     * @brief Get the format of the frame
     *
     * @return OBFormat returns the format of the frame
     *
     */
    virtual OBFormat format();

    /**
     *
     * @brief Get the sequence number of the frame
     *
     * @return uint64_t returns the sequence number of the frame
     *
     */
    virtual uint64_t index();

    /**
     *
     * @brief Get frame data
     *
     * @return void* returns frame data
     *
     */
    virtual void *data();

    /**
     *
     * @brief Get the frame data size
     *
     * @return uint32_t returns the frame data size
     * If it is point cloud data, it returns the number of bytes occupied by all point sets. If you need to find the number of points, you need to divide the
     * dataSize by the structure size of the corresponding point type.
     */
    virtual uint32_t dataSize();

    /**
     *
     * @brief Get the hardware timestamp of the frame
     *
     * @return uint64_t returns the time stamp of the frame hardware
     *
     */
    uint64_t timeStamp();

    /**
     *
     * @brief Get the hardware timestamp of the frame us
     *
     * @return uint64_t returns the time stamp of the frame hardware, unit us
     *
     */
    uint64_t timeStampUs();

    /**
     *
     * @brief Get frame system timestamp
     *
     * @return uint64_t returns the time stamp of the frame hardware
     *
     */
    uint64_t systemTimeStamp();

    /**
     * @brief Create a Frame object
     *
     * @param type frame type @ref OBFrameType
     * @param format frame format @ref OBFormat
     * @param width frame width
     * @param height frame height
     * @param strideBytes frame strideBytes, frame data size = height * strideBytes
     * @return std::shared_ptr<Frame> returns the created frame object
     */
    static std::shared_ptr<Frame> createFrame(OBFrameType type, OBFormat format, uint32_t width, uint32_t height, uint32_t strideBytes);

    /**
     *
     * @brief Check if the runtime type of the frame object is compatible with a given type
     *
     * @tparam T Given type
     * @return bool returns result
     *
     */
    template <typename T> bool is();

    /**
     *
     * @brief Frame object type conversion
     *
     * @tparam T Target type
     * @return std::shared_ptr<T> returns the result, if it cannot be converted, an exception will be thrown
     *
     */
    template <typename T> std::shared_ptr<T> as() {
        if(!is<T>())
            throw "unsupported operation, object's type is not require type";

        return std::static_pointer_cast<T>(std::const_pointer_cast<Frame>(shared_from_this()));
    }

private:
    friend class Filter;
    friend class Recorder;
    friend class FrameHelper;
};

class OB_EXTENSION_API VideoFrame : public Frame {
public:
    VideoFrame(Frame &frame);
    virtual ~VideoFrame() noexcept {};

    /**
     *
     * @brief Get frame width
     *
     * @return uint32_t returns the width of the frame
     *
     */
    uint32_t width();

    /**
     *
     * @brief Get frame height
     *
     * @return uint32_t returns the height of the frame
     *
     */
    uint32_t height();

    /**
     *
     * @brief Get the metadata of the frame
     *
     * @return void* returns the metadata of the frame
     *
     */
    void *metadata();

    /**
     *
     * @brief Get the metadata size of the frame
     *
     * @return uint32_t returns the metadata size of the frame
     *
     */
    uint32_t metadataSize();

    /**
     *
     * @brief Get the effective number of pixels (such as Y16 format frame, but only the lower 10 bits are valid bits, and the upper 6 bits are filled with 0)
     * @attention Only valid for Y8/Y10/Y11/Y12/Y14/Y16 format
     *
     * @return uint8_t returns the effective number of pixels in the pixel, or 0 if it is an unsupported format
     *
     */
    uint8_t pixelAvailableBitSize();
};

class OB_EXTENSION_API ColorFrame : public VideoFrame {
public:
    ColorFrame(Frame &frame);
    ~ColorFrame() noexcept {};
};

class OB_EXTENSION_API DepthFrame : public VideoFrame {
public:
    DepthFrame(Frame &frame);
    ~DepthFrame() noexcept {};

    /**
     *
     * @brief Get the value scale of the depth frame. The pixel value of depth frame is multiplied by the scale to give a depth value in millimeter.
     *      such as valueScale=0.1, and a certain coordinate pixel value is pixelValue=10000, then the depth value = pixelValue*valueScale = 10000*0.1=1000mm。
     *
     * @return float scale
     *
     */
    float getValueScale();
};

class OB_EXTENSION_API IRFrame : public VideoFrame {
public:
    IRFrame(Frame &frame);
    virtual ~IRFrame() noexcept {};

public:
    OBSensorType getDataSource();
};

class OB_EXTENSION_API PointsFrame : public Frame {
public:
    PointsFrame(Frame &frame);
    ~PointsFrame() noexcept {};

    /**
     *
     * @brief Get the point position value scale of the points frame. the point position value of points frame is multiplied by the scale to give a position
     * value in millimeter. such as scale=0.1, The x-coordinate value of a point is x = 10000, which means that the actual x-coordinate value = x*scale =
     * 10000*0.1 = 1000mm.
     *
     * @param[in] frame Frame object
     * @param[out] error Log error messages
     * @return float position value scale
     *
     */
    float getPositionValueScale();
};

class OB_EXTENSION_API FrameSet : public Frame {

public:
    // FrameSet();
    FrameSet(Frame &frame);
    ~FrameSet() noexcept;

    /**
     *
     * @brief Get frame count
     *
     * @return uint32_t returns the number of frames
     *
     */
    uint32_t frameCount();

    /**
     *
     * @brief Get depth frame
     *
     * @return std::shared_ptr<DepthFrame> returns the depth frame
     *
     */
    std::shared_ptr<DepthFrame> depthFrame();

    /**
     *
     * @brief Get color frame
     *
     * @return std::shared_ptr<ColorFrame> returns the color frame
     *
     */
    std::shared_ptr<ColorFrame> colorFrame();

    /**
     *
     * @brief Get infrared frame
     *
     * @return std::shared_ptr<IRFrame> returns infrared frame
     *
     */
    std::shared_ptr<IRFrame> irFrame();

    /**
     *
     * @brief Get point cloud frame
     *
     * @return  std::shared_ptr<PointsFrame> returns the point cloud data frame
     *
     */
    std::shared_ptr<PointsFrame> pointsFrame();

    /**
     *
     * @brief Get frame by sensor type
     *
     * @param frameType  Type of sensor
     * @return std::shared_ptr<Frame> returns the corresponding type of frame
     *
     */
    std::shared_ptr<Frame> getFrame(OBFrameType frameType);

    friend class Pipeline;
    friend class Filter;
};

class OB_EXTENSION_API AccelFrame : public Frame {
public:
    AccelFrame(Frame &frame);
    ~AccelFrame() noexcept = default;

    /**
     *
     * @brief Accelerometer frame data
     *
     * @return OBAccelValue
     *
     */
    OBAccelValue value();

    /**
     *
     * @brief Get the temperature when the frame is sampled
     *
     * @return float temperature value
     *
     */
    float temperature();
};

class OB_EXTENSION_API GyroFrame : public Frame {
public:
    GyroFrame(Frame &frame);
    ~GyroFrame() noexcept = default;

    /**
     *
     * @brief Get gyro frame data
     *
     * @return OBAccelValue
     *
     */
    OBGyroValue value();

    /**
     *
     * @brief Get temperature when the frame is sampled.
     *
     * @return float temperature value
     *
     */
    float temperature();
};

class OB_EXTENSION_API FrameHelper {
public:
    FrameHelper();
    ~FrameHelper();
    /**
     *
     * @brief Create a frame object based on an externally created Buffer
     *
     * @param format frame object format
     * @param frameWidth frame object width
     * @param frameHeight frame object height
     * @param buffer frame object buffer
     * @param bufferSize frame object buffer size
     * @return std::shared_ptr<Frame> return frame object
     *
     */
    static std::shared_ptr<Frame> createFrameFromBuffer(OBFormat format, uint32_t frameWidth, uint32_t frameHeight, uint8_t *buffer, uint32_t bufferSize,
                                                        BufferDestroyCallback destroyCallback, void *destroyCallbackContext);

    /**
     *
     * @brief Creates an empty frame collection object
     *
     * @return std::shared_ptr<Frame> return frameset object
     *
     */
    static std::shared_ptr<Frame> createFrameSet();

    /**
     *
     * @brief Populate the frame collection with frames of the corresponding type
     *
     * @param frameSet frameset object
     * @param frameType the type of frame filled in
     * @param frame the object that fills the frame
     *
     */
    static void pushFrame(std::shared_ptr<Frame> frameSet, OBFrameType frameType, std::shared_ptr<Frame> frame);

    /**
     *
     * @brief Set the system timestamp of the frame
     *
     * @param frame object for frame settings
     * @param systemTimestamp set by systemTimestamp
     *
     */
    static void setFrameSystemTimestamp(std::shared_ptr<Frame> frame, uint64_t systemTimestamp);

    /**
     * @brief Set the device timestamp of the frame
     *
     * @param frame object for frame settings
     * @param deviceTimestamp set by deviceTimestamp
     *
     */
    static void setFrameDeviceTimestamp(std::shared_ptr<Frame> frame, uint64_t deviceTimestamp);

    /**
     *
     * @brief Set the device timestamp of the frame
     *
     * @param frame object for frame settings
     * @param deviceTimestampUs the device timestamp set (Us)
     *
     */
    static void setFrameDeviceTimestampUs(std::shared_ptr<Frame> frame, uint64_t deviceTimestampUs);
};

template <typename T> bool Frame::is() {
    switch(this->type()) {
    case OB_FRAME_IR_LEFT:   // follow
    case OB_FRAME_IR_RIGHT:  // follow
    case OB_FRAME_IR:
        return (typeid(T) == typeid(IRFrame) || typeid(T) == typeid(VideoFrame));
    case OB_FRAME_DEPTH:
        return (typeid(T) == typeid(DepthFrame) || typeid(T) == typeid(VideoFrame));
    case OB_FRAME_COLOR:
        return (typeid(T) == typeid(ColorFrame) || typeid(T) == typeid(VideoFrame));
    case OB_FRAME_GYRO:
        return (typeid(T) == typeid(GyroFrame));
    case OB_FRAME_ACCEL:
        return (typeid(T) == typeid(AccelFrame));
    case OB_FRAME_SET:
        return (typeid(T) == typeid(FrameSet));
    case OB_FRAME_POINTS:
        return (typeid(T) == typeid(PointsFrame));
    default:
        std::cout << "ob::Frame::is() not catch frame type: " << (int)this->type() << std::endl;
        break;
    }
    return false;
}
}  // namespace ob
