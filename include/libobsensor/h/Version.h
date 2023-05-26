/**
 * @file Version.h
 * @brief Get the SDK version number information function
 *
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the SDK version
 *
 * @return int  returns the SDK version
 *
 */
int ob_get_version();

/**
 * @brief Get the SDK major version
 *
 * @return int returns the SDK major version
 *
 */
int ob_get_major_version();
/**
 * @brief Get the SDK minor version
 *
 * @return int returns the SDK minor version
 *
 */
int ob_get_minor_version();

/**
 * @brief Get the SDK patch version
 *
 * @return int returns the SDK patch version
 *
 */
int ob_get_patch_version();

/**
 * @brief Get sdk stage version.
 *     Note: Not need to free point char*
 *
 */
const char *ob_get_stage_version();

#ifdef __cplusplus
}
#endif
