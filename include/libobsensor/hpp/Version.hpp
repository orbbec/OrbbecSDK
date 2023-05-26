/**
 * @file Version.hpp
 * @brief Get the SDK version information
 *
 */
#pragma once

namespace ob {
class OB_EXTENSION_API Version {
public:
    /**
     * @brief Get the SDK major version number
     *
     * @return int returns the SDK major version number
     *
     */
    static int getMajor();
    /**
     * @brief Get the SDK minor version number
     *
     * @return int returns the SDK minor version number
     *
     */
    static int getMinor();
    /**
     * @brief Get the SDK revision number
     *
     * @return int returns the SDK revision number
     *
     */
    static int getPatch();

    /**
     * @brief Get the SDK version number
     *
     * @return int returns the SDK version number
     *
     */
    static int getVersion();

    /**
     * @brief Get sdk stage version.
     *
     */
    static std::string getStageVersion();
};
}  // namespace ob
