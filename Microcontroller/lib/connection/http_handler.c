#include "http_handler.h"
#include "frozen_json_helper.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "instruction_buffer.h"
#include "debug_helper.h"


#define INST_MAX_SIZE 20
static const char *TAG = "HTTP_HANDLER";
static const char *URL = "http://192.168.4.2:8080/instruction/last"; // Backend URL

static void decodeInstruction(int, char *);

esp_err_t client_event_get_handler(esp_http_client_event_handle_t evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        // printf("HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
        decodeInstruction(evt->data_len, (char *)evt->data);
        break;

    default:
        break;
    }
    return ESP_OK;
}

esp_err_t getHTTPInstruction(void)
{
    esp_err_t err;

    esp_http_client_config_t config = {
        .url = URL,
        .timeout_ms = 5000,
        .cert_pem = NULL,
        .event_handler = client_event_get_handler};
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client)
    {
        DEBUGING_ESP_LOG(ESP_LOGE(TAG, "Failed to initialize HTTP client"));
        return ESP_FAIL;
    }

    // Realizar la solicitud GET
    err = esp_http_client_perform(client);
    if (err != ESP_OK)
    {
        DEBUGING_ESP_LOG(ESP_LOGE(TAG, "ERROR PERFORMING GET %s",esp_err_to_name(err)));

        esp_http_client_cleanup(client);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        return ESP_FAIL;
    }
    esp_http_client_cleanup(client);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    return ESP_OK;
}

static void decodeInstruction(int length, char *str)
{
    char msg[INST_MAX_SIZE] = "";
    esp_err_t err = deserialize_json_data(str, msg, INST_MAX_SIZE);
    if (err != ESP_OK)
    {
        DEBUGING_ESP_LOG(ESP_LOGE(TAG, "ERORR DECODING JSON"));
    }
    else
    {
        DEBUGING_ESP_LOG(ESP_LOGW(TAG, "INST: %s", msg));
        if (strncmp(msg, "REBOOT", strlen("REBOOT")) == 0)
        {
            DEBUGING_ESP_LOG(ESP_LOGW(TAG, "Reiniciando el MCU..."));
            esp_restart(); // Llamada para reiniciar el MCU
        }
        else if (strncmp(msg, "ABORT", strlen("ABORT")) == 0)
        {
            DEBUGING_ESP_LOG(ESP_LOGW(TAG, "Abortando..."));
        }
        else
        {
            saveInstruction(msg);
        }
    }
}