/**
 * @file json_helper.h
 * @author Guerrico Leonel (lguerrico@outlook.com)
 * @brief JSON Helper Library Header for ESP32 using Frozen JSON
 *
 * This file provides the function declarations for creating, parsing, and printing
 * JSON objects using the `Frozen JSON` library. These functions operate with fixed buffers
 * to ensure memory efficiency in ESP32 applications.
 *
 * @version 1.0
 * @date 2024-02-03
 *
 * @note
 * - Uses a fixed buffer to avoid dynamic memory allocation.
 * - Ensure JSON strings are well-formed to avoid parsing errors.
 *
 */

#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include "esp_err.h"


/**
 * @brief Creates a JSON string from key-value pairs using Frozen JSON.
 *
 * @param[out] msg Buffer to store the JSON string.
 * @param[in] msg_size Size of the buffer.
 * @param[in] keys Array of keys for the JSON object.
 * @param[in] values Array of values corresponding to the keys.
 * @param[in] length Number of key-value pairs.
 *
 * @return
 * - `ESP_OK`: If the JSON string was successfully created.
 * - `ESP_FAIL`: If there was an error creating the JSON string.
 */
esp_err_t create_json_data(char **msg, const char **keys, const char **values, size_t length);

/**
 * @brief Deserializes a JSON string and extracts specific data using Frozen JSON.
 *
 * @param[in] data The JSON string to parse.
 * @param[out] msg Buffer to store the extracted instruction.
 * @param[in] msg_size Length of the output buffer.
 *
 * @return
 * - `ESP_OK`: If the data was successfully deserialized.
 * - `ESP_ERR_INVALID_ARG`: If expected keys are missing.
 * - `ESP_ERR_INVALID_SIZE`: If the extracted instruction exceeds the buffer size.
 * - `ESP_FAIL`: If parsing fails.
 */
esp_err_t deserialize_json_data(const char *data, char *msg, size_t msg_size);

/**
 * @brief Prints a JSON string to the log.
 *
 * @param[in] json_str The JSON string to print.
 */
void print_json_data(const char *json_str);


#endif /* JSON_HELPER_H */
