#include "http_handler.h"

static const char *TAG = "HTTP_HANDLER";                                       
static const char *URL =  "http://localhost:8080/instruction" // Backend URL


esp_err_t getHTTPInstruccion(char *inst, size_t inst_size)
{
    esp_err_t err;

    esp_http_client_config_t config = {
        .url = URL,
        .timeout_ms = 5000,                        
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client)
    {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        return ESP_FAIL;
    }

    // Realizar la solicitud GET
    err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        int http_status = esp_http_client_get_status_code(client);
        if (http_status == 200)
        {
            int content_length = esp_http_client_get_content_length(client);
            if (content_length > 0 && content_length < inst_size)
            {
                char response[content_length + 1];
                esp_http_client_read(client, response, content_length);
                response[content_length] = '\0'; 

                // Parsear el JSON
                cJSON *json = cJSON_Parse(response);
                if (json == NULL)
                {
                    ESP_LOGE(TAG, "Failed to parse JSON");
                    esp_http_client_cleanup(client);
                    return ESP_ERR_INVALID_RESPONSE;
                }

                // Extraer la instruccion del JSON
                cJSON *instruction = cJSON_GetObjectItem(json, "instruction");
                if (cJSON_IsString(instruction))
                {
                    snprintf(inst, inst_size, "%s", instruction->valuestring);
                    cJSON_Delete(json);
                    esp_http_client_cleanup(client);
                    return ESP_OK;
                }
                else
                {
                    ESP_LOGE(TAG, "Invalid JSON format");
                }

                cJSON_Delete(json);
            }
            else
            {
                ESP_LOGE(TAG, "Invalid response size");
            }
        }
        else
        {
            ESP_LOGE(TAG, "HTTP status code: %d", http_status);
        }
    }
    else
    {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    return ESP_FAIL;
}
