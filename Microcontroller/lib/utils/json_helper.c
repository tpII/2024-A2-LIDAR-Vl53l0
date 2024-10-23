#include "json_helper.h"
#include "esp_log.h"

#include <string.h>
#include <stdlib.h>

static const char *TAG = "JSON_HELPER";

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

esp_err_t deserealize_json_data(const char *data, char *msg, const size_t message_length)
{

    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
    {
        ESP_LOGE("JSON", "Error parsing JSON data");
        return ESP_FAIL;
    }

    // Verificar que el JSON tiene las claves esperadas
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
    const char *time = cJSON_GetStringValue(time_json);

    if (instruction == NULL || time == NULL)
    {
        ESP_LOGE("JSON", "Error getting values for 'instruction' or 'time'");
        cJSON_Delete(json); // Limpiar el objeto JSON
        return ESP_FAIL;
    }

    // Loggear las claves y valores para debug
    ESP_LOGW(TAG, "Key: instruction -> value: %s", instruction);
    ESP_LOGW(TAG, "Key: time -> value: %s", time);

    // Validar el tamaño del mensaje
    if (strlen(instruction) > message_length)
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