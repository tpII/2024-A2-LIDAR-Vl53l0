#include "freertos.h"

static const char *TAG = "FreeRTOS";

static void access_point(void *pvParameters);
static void http_server(void *pvParameters);

esp_err_t complete_initialization(void){
    esp_err_t err = initialize_access_point();
    if (err != ESP_OK) {
        return ESP_FAIL;
    }

    err = initialize_http_server();
    if (err != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

// create tasks
esp_err_t initialize_access_point(void)
{
    xTaskCreatePinnedToCore(access_point, "AP Server", 4096, NULL, 1, NULL, 0);
    return ESP_OK;
}

esp_err_t initialize_http_server(void){
    xTaskCreatePinnedToCore(http_server, "HTTP Server", 4096, NULL, 1, NULL, 0);
    return ESP_OK;
}

// tasks
static void access_point(void *pvParameters)
{
    esp_err_t err = initialize_server();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize AP server: err %d", err);
    }
}

static void http_server(void *pvParameters)
{
    esp_err_t err = start_http_server();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize HTTP server: err %d", err);
    }
}