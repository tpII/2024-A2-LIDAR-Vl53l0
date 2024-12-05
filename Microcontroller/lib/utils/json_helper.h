/**
 * @file json_helper.h
 * @brief JSON Helper Library for ESP32
 * 
 * This library provides utility functions for creating, parsing, and printing JSON objects. 
 * 
 * @version 1.0
 * @date 2024-12-05
 * 
 * @note
 * - Memory management is crucial when working with dynamically allocated JSON strings.
 * - Ensure that the `cJSON` library is included in the project.
 * 
 * @author Guerrico Leonel (lguerrico@outlook.com)
 * 
 */
#ifndef _JSON_HELPER_H_
#define _JSON_HELPER_H_
#include "cJSON.h"
#include "esp_err.h"

/**
 * @brief Creates a JSON string from key-value pairs.
 * 
 * This function generates a JSON string from an array of keys and values. 
 * The resulting JSON string is dynamically allocated and must be freed by the caller.
 * 
 * @param[out] msg Pointer to the JSON string. The caller is responsible for freeing this memory.
 * @param[in] keys Array of keys for the JSON object.
 * @param[in] values Array of values corresponding to the keys.
 * @param[in] length Number of key-value pairs.
 * 
 * @return 
 * - `ESP_OK`: If the JSON string was successfully created.
 * - `ESP_FAIL`: If there was an error creating the JSON object or adding key-value pairs.
 */
esp_err_t create_json_data(char **, const char **, const char **, const size_t);

/**
 * @brief Deserializes a JSON string and extracts specific data.
 * 
 * This function parses a JSON string and extracts the values associated with specific keys. 
 * It validates the input JSON structure and ensures that extracted data fits within the provided buffer.
 * 
 * @param[in] data The JSON string to parse.
 * @param[out] msg Buffer to store the extracted instruction.
 * @param[in] message_length Length of the output buffer.
 * 
 * @return 
 * - `ESP_OK`: If the data was successfully deserialized.
 * - `ESP_ERR_INVALID_ARG`: If expected keys are missing in the JSON string.
 * - `ESP_ERR_INVALID_SIZE`: If the extracted instruction exceeds the buffer size.
 * - `ESP_FAIL`: If parsing fails or the data cannot be retrieved.
 */
esp_err_t deserealize_json_data(const char *,char *, const size_t);

/**
 * @brief Prints a JSON string to the log.
 * 
 * This function logs the given JSON string for debugging purposes. If the string is null, 
 * it logs an error message.
 * 
 * @param[in] json_str The JSON string to print.
 */
void print_json_data(const char *);

#endif