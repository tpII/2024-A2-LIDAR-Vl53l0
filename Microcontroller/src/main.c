#include "esp_log.h"
//#include "ap_server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "mqtt_server.h"

//#include "mqtt_handler.h"

#include "vl53l0x.h"

#include <stdio.h>
#include <string.h>

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
        err = i2c_master_init();
        if(err != ESP_OK){
                ESP_LOGE(TAG, "Error en el i2c_master_init: %s", esp_err_to_name(err));
                return 2;
        }

        #if defined VL53L0X
                ESP_LOGI(TAG, "Iniciando Lectura inicial...");
                bool success = vl53l0x_init();
                uint16_t ranges[3] = { 0 };
                while (success) {
                        success = vl53l0x_read_range_single(VL53L0X_IDX_FIRST, &ranges[0]);
                        if (ranges[0] == VL53L0X_OUT_OF_RANGE){
                                ESP_LOGI(TAG, "Distancia: Out Of Range");
                        }
                        else{
                                ESP_LOGI(TAG, "Distancia: %d", ranges[0]);
                        }
                        vTaskDelay(pdMS_TO_TICKS(100));
                #ifdef VL53L0X_SECOND
                        success &= vl53l0x_read_range_single(VL53L0X_IDX_SECOND, &ranges[1]);
                #endif
                #ifdef VL53L0X_THIRD
                        success &= vl53l0x_read_range_single(VL53L0X_IDX_THIRD, &ranges[2]);
                #endif
                }
        #endif
        return 0;
}
