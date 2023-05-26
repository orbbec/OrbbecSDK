/**
 * @file  Filter.hpp
 * @brief The processing unit of the SDK can perform point cloud generation, format conversion and other functions.
 *
 */
#pragma once

#include "Types.hpp"
#include <functional>
#include <memory>

namespace ob {
class Frame;

typedef std::function<void(std::shared_ptr<Frame>)> FilterCallback;

class OB_EXTENSION_API Filter {
public:
    Filter();

    virtual ~Filter() = default;

    /**
     * @brief filter reset, free the internal cache, stop the processing thread and clear the pending buffer frame when asynchronous processing
     *
     */
    virtual void reset();

    /**
     * @brief Processing frames (synchronous interface)
     *
     * @param frame frame to be processed
     * @return std::shared_ptr< Frame > processed frame
     *
     */
    virtual std::shared_ptr<Frame> process(std::shared_ptr<Frame> frame);

    /**
     * @brief Push the pending frame into the cache (asynchronous callback interface)
     *
     * @param frame The pending frame processing result is returned by the callback function
     *
     */

    virtual void pushFrame(std::shared_ptr<Frame> frame);

    /**
     * @brief Set the callback function (asynchronous callback interface)
     *
     * @param callback Processing result callback
     *
     */
    virtual void setCallBack(FilterCallback callback);

protected:
    std::shared_ptr<FilterImpl> impl_;
};

class OB_EXTENSION_API PointCloudFilter : public Filter {
public:
    PointCloudFilter();
    /**
     * @brief Set point cloud type parameters
     *
     * @param type Point cloud type    depth point cloud or RGBD point cloud
     *
     */
    void setCreatePointFormat(OBFormat type);

    /**
     * @brief Set camera parameters
     *
     * @param param Camera internal and external parameters
     *
     */
    void setCameraParam(OBCameraParam param);

    /**
     * @brief  Set the frame alignment state that will be input to generate point cloud (it needs to be enabled in D2C mode, as the basis for the algorithm to
     * select the set of camera internal parameters)
     *
     * @param state Alignment status, True: enable alignment; False: disable alignment
     *
     */
    void setFrameAlignState(bool state);

    /**
     * @brief  Set the point cloud coordinate data zoom factor
     *
     * @attention Calling this function to set the scale will change the point coordinate scaling factor of the output point cloud frame: posScale = posScale /
     * scale.The point coordinate scaling factor for the output point cloud frame can be obtained via @ref PointsFrame::getPositionValueScale function
     *
     * @param scale Zoom factor
     *
     */
    void setPositionDataScaled(float scale);

    /**
     * @brief  Set point cloud color data normalization
     *
     * @param state Whether normalization is required
     *
     */
    void setColorDataNormalization(bool state);

    /**
     * @brief  Set point cloud coordinate system
     *
     * @param type coordinate system type
     *
     */
    void setCoordinateSystem(OBCoordinateSystemType type);
};

class OB_EXTENSION_API FormatConvertFilter : public Filter {
public:
    FormatConvertFilter();
    /**
     * @brief Set format conversion type
     *
     * @param type Format conversion type
     *
     */
    void setFormatConvertType(OBConvertFormat type);
};

class OB_EXTENSION_API CompressionFilter : public Filter {
public:
    CompressionFilter();
    /**
     * @brief Set compression params
     *
     * @param mode Compression mode OB_COMPRESSION_LOSSLESS or OB_COMPRESSION_LOSSY
     * @param params Compression params, when mode is OB_COMPRESSION_LOSSLESS, params is NULL
     *
     */
    void setCompressionParams(OBCompressionMode mode, void *params);
};

class OB_EXTENSION_API DecompressionFilter : public Filter {
public:
    DecompressionFilter();
};

}  // namespace ob
