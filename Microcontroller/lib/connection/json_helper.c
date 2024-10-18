#include "json_helper.h"
#include "esp_log.h"

#include <string.h>
#include <stdlib.h>

static const char *TAG = "JSON_HELPER";

esp_err_t create_json_data(char **msg, const char **keys, const char **values, const size_t lenght)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
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

    // Ensure there are enough keys and values
    if (
        cJSON_GetArraySize(json) != 1)
    {
        ESP_LOGE("JSON", "Length mismatch between espected message and JSON object");
        cJSON_Delete(json); // Clean up
        return ESP_ERR_INVALID_SIZE;
    }

    // Obtener el tamaño del JSON (número de elementos)
    size_t num_items = cJSON_GetArraySize(json);
    if (num_items == 0)
    {
        ESP_LOGE("JSON", "No se encontraron elementos en el JSON");
        cJSON_Delete(json);
        return ESP_FAIL;
    }

   
    // Iterate through the JSON object and fill the keys and values arrays
    const char *instruccion = NULL;
    const char *time = NULL;

    cJSON *item = NULL;
    
    cJSON_ArrayForEach(item, json)
    {
        const char *key = item->string;
        const char *value = cJSON_GetStringValue(item);

        if(key == NULL || value == NULL){
            ESP_LOGE("JSON", "Error getting key or value from JSON object");
            cJSON_Delete(json);
            return ESP_FAIL;
        }

        if (strcmp(key, "instruccion") == 0)
        {
            instruccion = value;
        }
        else if (strcmp(key, "time") == 0)
        {
            time = value;
        }
    }

    if(instruccion == NULL || time == NULL){
        ESP_LOGE("JSON", "Error getting instruccion or time from JSON object");
        cJSON_Delete(json);
        return ESP_FAIL;
    }

    if(strlen(instruccion) > message_length || strlen(time) > message_length){
        ESP_LOGE("JSON", "Message length exceeds buffer size");
        cJSON_Delete(json);
        return ESP_ERR_INVALID_SIZE;
    }

    strncpy(msg, instruccion, message_length);

    // Clean up JSON object
    cJSON_Delete(json);

    return ESP_OK;
}
/*
void print_json_data(cJSON *json) {
    if (json != NULL) {
        // Convertir el JSON a una cadena de caracteres
        char *json_str = cJSON_Print(json);
        if (json_str != NULL) {
            // Imprimir el JSON
            ESP_LOGI(TAG, "JSON creado: %s", json_str);
            // Liberar la memoria después de usarla
            cJSON_free(json_str);
        } else {
            ESP_LOGE(TAG, "Error al convertir JSON a cadena");
        }
    } else {
        ESP_LOGE(TAG, "JSON es NULL");
    }
}
*/

void print_json_data(const char *json_str) {
    if (json_str != NULL) {
        // Imprimir el JSON que ya está en formato de cadena
        ESP_LOGI(TAG, "JSON creado: %s", json_str);
    } else {
        ESP_LOGE(TAG, "JSON es NULL o no válido");
    }
}