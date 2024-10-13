#include "http_client.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "HTTP_CLIENT";
static const char *URL = "http://192.168.4.2:8080/";

static esp_err_t create_json_data(char **, const char **, const char **, const size_t);
static esp_err_t deserealize_json_data(const char *,char *, const size_t);

esp_err_t http_post(const char *message_type, const char **keys, const char **values, const size_t length)
{

    char full_url[512];
    snprintf(full_url, sizeof(full_url), "%s%s", URL, message_type);  // Concatenar URL
    ESP_LOGI(TAG,"ENDPOINT TO CALL: %s",full_url);
    char *json_data = NULL;
    esp_err_t err = create_json_data(&json_data, keys, values, length);
    if (err != ESP_OK || json_data == NULL)
    {
        ESP_LOGE(TAG, "Error creating JSON data: %s", esp_err_to_name(err));
        return ESP_FAIL;
    }

    esp_http_client_config_t config = {
        .url = full_url
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, json_data, strlen(json_data));

    // Realizar la solicitud HTTP POST
    esp_err_t err2 = esp_http_client_perform(client);

    if (err2 == ESP_OK)
    {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %lld",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err2));
        return ESP_FAIL;
    }

    esp_http_client_cleanup(client);
    free(json_data); // Liberar memoria asignada para el JSON
    return ESP_OK;
}

esp_err_t http_get(const char *message_type, char *msg, size_t length)
{
    char full_url[512];
    snprintf(full_url, sizeof(full_url), "%s%s", URL, message_type);  // Concatenar URL
    ESP_LOGI(TAG,"ENDPOINT TO CALL: %s",full_url);
    esp_http_client_config_t config = {
        .url = full_url,
        .method = HTTP_METHOD_GET,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG, "Error initializing HTTP client");
        return ESP_FAIL;
    }
    esp_http_client_set_header(client, "Content-Type", "application/json");
    
    // Realizar la solicitud HTTP GET
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %lld",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));

        // Obtener la respuesta del servidor
        int content_length = esp_http_client_get_content_length(client);
        if (content_length > 0)
        {
            // Reservar memoria para la respuesta JSON
            char *buffer = malloc(content_length + 1);
            if (buffer == NULL) {
                ESP_LOGE(TAG, "Failed to allocate memory for response");
                esp_http_client_cleanup(client);
                return ESP_ERR_NO_MEM;
            }

            // Leer el cuerpo de la respuesta
            esp_http_client_read(client, buffer, content_length);
            buffer[content_length] = '\0'; // Asegurarse de que la cadena esté terminada en NULL

            ESP_LOGI(TAG, "HTTP GET Response: %s", buffer);

            // Deserializar el JSON utilizando la función deserealize_json_data
            esp_err_t deserialization_err = deserealize_json_data(buffer, msg, length);
            if (deserialization_err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error deserializing JSON data");
                free(buffer); // Liberar memoria asignada para la respuesta
                esp_http_client_cleanup(client);
                return ESP_FAIL;
            }

            // Liberar la memoria después de usarla
            free(buffer);
        }
        else
        {
            ESP_LOGE(TAG, "No content in HTTP GET response");
            esp_http_client_cleanup(client);
            return 1;
        }
    }
    else
    {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return err;
    }

    esp_http_client_cleanup(client);
    return ESP_OK;
}

static esp_err_t create_json_data(char **msg, const char **keys, const char **values, const size_t lenght)
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
            cJSON_Delete(root); // Limpiar memoria si hay error al añadir elementos
            return ESP_FAIL;
        }
    }

    *msg = cJSON_Print(root);
    cJSON_Delete(root);
    if (*msg == NULL)
    {
        return ESP_FAIL; // Error si no se puede generar el string JSON
    }
    

    return ESP_OK; // Retornar éxito
}

static esp_err_t deserealize_json_data(const char *data, char *msg, const size_t message_length)
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