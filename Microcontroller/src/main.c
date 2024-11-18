#include "esp_log.h"
//#include "ap_server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "mqtt_server.h"

//#include "mqtt_handler.h"

#include "vl53l0x.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>
#include "ina219.h"
static const char *TAG = "MAIN";


#define VL53L0X
//#define VL61L0X



int app_main(void){   
        esp_err_t err;

        ESP_LOGI(TAG, "Iniciando GPIO...");
        err = gpio_init();
        if(err != ESP_OK){
                ESP_LOGE(TAG, "Error en el gpio_init: %s", esp_err_to_name(err));
                return 1;
        }

        ESP_LOGI(TAG, "Iniciando I2C...");
        err = i2c_init();
        if(err != ESP_OK){
                ESP_LOGE(TAG, "Error en el i2c_master_init: %s", esp_err_to_name(err));
                return 2;
        }

       return 0;
}
