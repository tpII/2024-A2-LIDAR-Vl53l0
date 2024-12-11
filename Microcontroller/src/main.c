#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "battery.h"
#include "mapping.h"
static const char *TAG = "MAIN";

#define VL53L0X
// #define VL61L0X

int app_main(void)
{
        esp_err_t err;

        err = battery_sensor_init();

        if (err != ESP_OK)
        {
                ESP_LOGE(TAG, "Error initializing battery sensor");
                return 1;
        }

        uint8_t level;
        while (1)
        {
                err = battery_sensor_read(&level);
                ESP_LOGI(TAG, "Baterry Level: %u", level);
                //vTaskDelay(1000);
        }

        if (mapping_init() != ESP_OK)
        {
                ESP_LOGE(TAG, "ERROR INTI LiDAR");
                return 1;
        };

        uint16_t x = 0, y = 0;
        while (1)
        {
                err = getMappingValue(&x, &y);
                ESP_LOGI(TAG, "Mapping x: %u - y: %u", x, y);
                vTaskDelay(1000);
                if (err != ESP_OK)
                {
                        ESP_LOGW(TAG, "ERROR MAPPING");
                        return 1;
                }
        }
        return 0;
}
