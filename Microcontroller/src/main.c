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

       return 0;
}
