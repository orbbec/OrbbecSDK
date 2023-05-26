/**
 * \@file Types.hpp
 * \@brief  Provide SDK structure and enumeration constant definition (depending on libobsensor/h/ObTypes.h)
 *
 */
#pragma once

#include "libobsensor/h/ObTypes.h"

#include <functional>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief file transfer callbacks
 *
 * @param state file transfer status
 * @param message status information
 */
using SendFileCallback = std::function<void(OBFileTranState state, const char *message, uint8_t percent)>;

/**
 * @brief device upgrade callback
 *
 * @param state upgrade status
 * @param message status information
 */
using DeviceUpgradeCallback = std::function<void(OBUpgradeState state, const char *message, uint8_t percent)>;

/**
 * @brief device status callback
 *
 * @param state device status
 * @param message status information
 */
using DeviceStateChangedCallback = std::function<void(OBDeviceState state, const char *message)>;

/**
 * @brief get raw data property data when data and progress callbacks
 *
 * @param dataChunk data chunk
 * @param state get the data status
 */
using GetDataCallback = std::function<void(OBDataTranState state, OBDataChunk *dataChunk)>;

/**
 * @brief set the raw data property when the progress callback is made
 *
 * @param percent  progress percentage
 * @param state  set the data status
 */
using SetDataCallback = std::function<void(OBDataTranState state, uint8_t percent)>;

#ifdef __cplusplus
}
#endif
