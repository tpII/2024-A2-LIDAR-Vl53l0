#include "mapping.h"
#include "servo.h"
#include "vl53l0x.h"
#include "esp_log.h"

#define MIN_DISTANCE 100

static const char *TAG = "MAPPING";

esp_err_t mapping_init()
{

    esp_err_t err = ESP_OK;
    ESP_LOGI(TAG, "Initializing Mapping");

    ESP_LOGI(TAG, "Initializing GPIO...");
    err = gpio_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error in gpio_init: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Initializing I2C...");
    err = i2c_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing I2C");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Initializing LiDAR...");
    if (!vl53l0x_init())
    {
        ESP_LOGE(TAG, "Error initializing LiDAR(VL53L0X)");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Initializing ServoMotor...");
    err = servo_initialize();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing Servo");
        return ESP_FAIL;
    }

    return err;
}

esp_err_t getMappingValue(uint16_t *angle, uint16_t *distance)
{

    if (angle == NULL || distance == NULL)
    {
        ESP_LOGE(TAG, "NULL pointer passed to getMappingValue.");
        return ESP_ERR_INVALID_ARG;
    }

    bool success = false;
    uint16_t value = 0;
    *angle = readAngle();
#ifndef VL53L0X
    success = vl53l0x_read_range_single(VL53L0X_IDX_FIRST, &value);
    if (success && (value != VL53L0X_OUT_OF_RANGE || value >= MIN_DISTANCE))
    {
        *distance = value;
        return ESP_OK;
    }
    return ESP_ERR_INVALID_ARG;
#else
    ESP_LOGE(TAG, "ERROR VL53L0X NOT DEFINED");
    return ESP_FAIL;
#endif
}
