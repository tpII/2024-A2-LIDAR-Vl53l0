#include "freertos.h"

static const char *TAG = "FreeRTOS";

static void access_point(void *pvParameters);

esp_err_t initialize_access_point(void)
{
    xTaskCreatePinnedToCore(access_point, "AP Server", 4096, NULL, 1, NULL, 0);
    return ESP_OK;
}

static void access_point(void *pvParameters)
{
    esp_err_t err = initialize_server();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize AP server: err %d", err);
    }
}
