/**
 * @file json_helper_frozen.c
 * @brief Implementation of JSON Helper Library for ESP32 using Frozen JSON
 *
 * This file contains the implementation of utility functions for creating,
 * parsing, and printing JSON objects using the `Frozen JSON` library. These functions
 * simplify working with JSON in ESP32 applications.
 *
 * @version 1.0
 * @date 2024-12-05
 *
 * @note
 * - Uses a fixed buffer to avoid dynamic memory allocation.
 * - Ensure JSON strings are well-formed to avoid parsing errors.
 *
 * @author Guerrico Leonel (lguerrico@outlook.com)
 *
 */
#include "frozen_json_helper.h"
#include "esp_log.h"
#include "frozen.h"

#include <string.h>

/**
 * @brief Log tag used for debugging and error messages.
 */
static const char *TAG = "JSON_HELPER";

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
esp_err_t create_json_data(char **msg, const char **keys, const char **values, const size_t length) {
    struct json_out out = JSON_OUT_BUF(NULL, 0); // Inicializa buffer vacío
    size_t size = 0;

    // Calcula el tamaño del JSON sin generar la cadena aún
    int estimated_size = json_printf(&out, "{");
    for (size_t i = 0; i < length; i++) {
        estimated_size += json_printf(&out, "%Q:%Q%s", keys[i], values[i], (i < length - 1) ? "," : "");
    }
    estimated_size += json_printf(&out, "}");

    // Asigna memoria dinámica
    size = estimated_size + 1; // +1 para el null-terminator
    *msg = (char *)malloc(size);
    if (*msg == NULL) {
        ESP_LOGE(TAG, "Error allocating memory for JSON string");
        return ESP_FAIL;
    }

    // Genera el JSON en el buffer asignado
    struct json_out out_real = JSON_OUT_BUF(*msg, size);
    json_printf(&out_real, "{");
    for (size_t i = 0; i < length; i++) {
        json_printf(&out_real, "%Q:%Q%s", keys[i], values[i], (i < length - 1) ? "," : "");
    }
    json_printf(&out_real, "}");

    ESP_LOGI(TAG, "JSON created: %s", *msg); // Log para verificar salida

    return ESP_OK;
}


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
#include "esp_log.h"
#include "frozen.h"

#define TAG "JSON_PARSER"

esp_err_t deserialize_json_data(const char *data, char *msg, size_t msg_size) 
{
    char *instruction = NULL;
    char *id = NULL;
    char *time = NULL;
    bool read = false;

    ESP_LOGE(TAG, "Received JSON: %s", data);

    int parsed = json_scanf(data, strlen(data), 
                            "{id:%Q, instruction:%Q, time:%Q, read:%B}", 
                            &id, &instruction, &time, &read);


    if (instruction == NULL) {
        ESP_LOGE(TAG, "Instruction is missing in JSON");
        free(id);
        free(time);
        return ESP_ERR_INVALID_ARG;
    }

    if (strlen(instruction) >= msg_size) {
        ESP_LOGE(TAG, "Message length exceeds buffer size");
        free(id);
        free(instruction);
        free(time);
        return ESP_ERR_INVALID_SIZE;
    }

    strncpy(msg, instruction, msg_size - 1);
    msg[msg_size - 1] = '\0'; // Asegurar terminación nula

    ESP_LOGI(TAG, "Final parsed values -> Instruction: %s, Time: %s", instruction, time);

    // Liberar memoria asignada dinámicamente
    free(id);
    free(instruction);
    free(time);

    return ESP_OK;
}



/**
 * @brief Prints a JSON string to the log.
 *
 * @param[in] json_str The JSON string to print.
 */
void print_json_data(const char *json_str)
{
    if (json_str != NULL)
    {
        ESP_LOGI(TAG, "JSON created: %s", json_str);
    }
    else
    {
        ESP_LOGE(TAG, "Invalid JSON string");
    }
}
