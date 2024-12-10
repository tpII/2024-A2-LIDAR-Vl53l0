#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mapping.h"

static const char *TAG = "MAIN";

#define VL53L0X
// #define VL61L0X

int app_main(void)
{
        esp_err_t err;

        ESP_LOGI(TAG, "Iniciando Mapping Service...");
        err = mapping_init();
        if (err != ESP_OK)
        {
                ESP_LOGE(TAG, "Error Iniciando Mapping:  %s", esp_err_to_name(err));
                return 1;
        }

        uint16_t angle = 0;
        uint16_t distance = 0;
        while (1)
        {
                err = getMappingValue(&angle,&distance);
                if(err != ESP_OK){
                        ESP_LOGE(TAG,"FAIL TO GET MAPPING VALUE");
                }
                ESP_LOGW(TAG,"Mapping Values: %d° - %dmm",angle,distance);
                vTaskDelay(200);
        }

        return 0;
}
