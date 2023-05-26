/**
 * @file  Error.hpp
 * @brief Error describes the abnormal error within the SDK, and the detailed information of the exception can be realized through this class.
 *
 */
#pragma once

#include "Types.hpp"
#include <memory>

struct ErrorImpl;

namespace ob {
class OB_EXTENSION_API Error {
private:
    std::unique_ptr<ErrorImpl> impl_;

public:
    Error(std::unique_ptr<ErrorImpl> impl) noexcept;

    Error(const Error &error) noexcept;

    ~Error() noexcept;
    /**
     * @brief Obtain detailed error logs of SDK internal exceptions.
     *
     */
    const char *getMessage() const noexcept;

    /**
     * @brief Get the exception type of the error, and determine which module is abnormal.
     * @return OBExceptionType
     *
     */
    OBExceptionType getExceptionType() const noexcept;

     /**
     * @brief Get the name of the error function.
     *
     */
    const char *getName() const noexcept;

    /**
     * @brief Get the parameter passed in to the error interface.
     *
     */
    const char *getArgs() const noexcept;
};
}  // namespace ob
