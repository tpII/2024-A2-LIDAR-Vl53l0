/**
 * @file json_helper.c
 * @brief Implementation of JSON Helper Library for ESP32
 *
 * This file contains the implementation of utility functions for creating,
 * parsing, and printing JSON objects using the `cJSON` library. These functions
 * simplify working with JSON in ESP32 applications.
 *
 * @version 1.0
 * @date 2024-12-05
 *
 * @note
 * - Dynamically allocated JSON strings must be freed by the caller.
 * - Ensure JSON strings are well-formed to avoid parsing errors.
 *
 * @author Guerrico Leonel (lguerrico@outlook.com)
 *
 */
#include "json_helper.h"
#include "esp_log.h"

#include <string.h>
#include <stdlib.h>

/**
 * @brief Log tag used for debugging and error messages.
 */
static const char *TAG = "JSON_HELPER";

/**
 * @brief Creates a JSON string from key-value pairs.
 *
 * Generates a JSON string from an array of keys and values. The resulting string
 * is dynamically allocated, and the caller is responsible for freeing it.
 *
 * @param[out] msg Pointer to the dynamically allocated JSON string.
 * @param[in] keys Array of keys for the JSON object.
 * @param[in] values Array of values corresponding to the keys.
 * @param[in] length Number of key-value pairs.
 *
 * @return
 * - `ESP_OK`: If the JSON string was successfully created.
 * - `ESP_FAIL`: If there was an error creating the JSON object or adding key-value pairs.
 */
esp_err_t create_json_data(char **msg, const char **keys, const char **values, const size_t lenght)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Error creating JSON object");
        return ESP_FAIL;
    }

    for (size_t i = 0; i < lenght; i++)
    {
        if (!cJSON_AddStringToObject(root, keys[i], values[i]))
        {
            ESP_LOGE(TAG, "Error adding key-value to JSON");
            cJSON_Delete(root); // Limpiar memoria si hay error al añadir elementos
            return ESP_FAIL;
        }
    }

    *msg = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if (*msg == NULL)
    {
        ESP_LOGE(TAG, "Error converting JSON to string");
        return ESP_FAIL; // Error si no se puede generar el string JSON
    }

    return ESP_OK; // Retornar éxito
}

/**
 * @brief Deserializes a JSON string and extracts specific data.
 *
 * Parses a JSON string, validates its structure, and extracts values for the keys
 * "instruction" and "time". Logs extracted data and validates buffer size for safe copying.
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
esp_err_t deserealize_json_data(const char *data, char *msg, const size_t message_length)
{

    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
    {
        ESP_LOGE("JSON", "Error parsing JSON data");
        return ESP_FAIL;
    }
    // ESP_LOGW(TAG,"JSON: %s",cJSON_Print(json));
    //  Verificar que el JSON tiene las claves esperadas
    cJSON *instruction_json = cJSON_GetObjectItem(json, "instruction");
    cJSON *time_json = cJSON_GetObjectItem(json, "time");

    if (instruction_json == NULL || time_json == NULL)
    {
        ESP_LOGE("JSON", "Missing expected keys 'instruction' or 'time'");
        cJSON_Delete(json); // Limpiar el objeto JSON
        return ESP_ERR_INVALID_ARG;
    }

    // Obtener los valores
    const char *instruction = cJSON_GetStringValue(instruction_json);
    if (instruction == NULL)
    {
        ESP_LOGE("JSON", "Error getting value for 'instruction'");
        cJSON_Delete(json); // Limpiar el objeto JSON
        return ESP_FAIL;
    }

    // Obtener el valor de 'time' como número
    long long time = cJSON_GetNumberValue(time_json);
    if (time == 0 && cJSON_IsNumber(time_json) == 0) // Verificar si es número
    {
        ESP_LOGE("JSON", "Error getting value for 'time'");
        cJSON_Delete(json); // Limpiar el objeto JSON
        return ESP_FAIL;
    }
    // Loggear las claves y valores para debug
    ESP_LOGW(TAG, "Key: instruction -> value: %s", instruction);
    ESP_LOGW(TAG, "Key: time -> value: %lld", time);

    // Validar el tamaño del mensaje
    if (strlen(instruction) >= message_length)
    {
        ESP_LOGE("JSON", "Message length exceeds buffer size");
        cJSON_Delete(json);
        return ESP_ERR_INVALID_SIZE;
    }

    // Copiar el valor de la instrucción al buffer
    strncpy(msg, instruction, message_length);
    
    // Limpiar el objeto JSON
    cJSON_Delete(json);

    return ESP_OK;
}

/**
 * @brief Prints a JSON string to the log.
 *
 * Logs the given JSON string. If the string is null, logs an error message.
 *
 * @param[in] json_str The JSON string to print.
 *
 * @note This function is mainly use for debug
 */
void print_json_data(const char *json_str)
{
    if (json_str != NULL)
    {
        // Imprimir el JSON que ya está en formato de cadena
        ESP_LOGI(TAG, "JSON creado: %s", json_str);
    }
    else
    {
        ESP_LOGE(TAG, "JSON es NULL o no válido");
    }
}