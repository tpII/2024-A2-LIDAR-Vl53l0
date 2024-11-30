#include "mapping.h"
#include "servo.h"
#include "vl53l0x.h"
#include "esp_log.h"

static const char *TAG = "MAPPING";

esp_err_t mapping_init()
{

    esp_err_t err = ESP_OK;
    ESP_LOGI(TAG, "Initializing Mapping");

    ESP_LOGI(TAG, "Iniciando GPIO...");
    err = gpio_init();
    if (gpio_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "Error en el gpio_init: %s", esp_err_to_name(err));
        return err;
    }

    

    if (!vl53l0x_init())
    {
        ESP_LOGE(TAG, "Error initializing LiDAR(VL53L0X)");
        return ESP_FAIL;
    }

    return err;
}