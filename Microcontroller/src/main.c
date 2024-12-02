#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "battery.h"

static const char *TAG = "MAIN";


#define VL53L0X
//#define VL61L0X



int app_main(void){   
        esp_err_t err;

        err = battery_sensor_init();

        if(err != ESP_OK){
                ESP_LOGE(TAG,"Error initializing battery sensor");
                return 1;
        }

        uint8_t level;
        while(1){
                err = battery_sensor_read(&level);
                ESP_LOGI(TAG,"Baterry Level: %u",level);
                vTaskDelay(1000);
        }

       return 0;
}
