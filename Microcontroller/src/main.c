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
  ESP_LOGI(TAG, "Iniciando GPIO...");
  gpio_init();

  ESP_LOGI(TAG, "Iniciando I2C...");
	i2c_master_init();
  #if defined VL53L0X
      ESP_LOGI(TAG, "Iniciando Lectura inicial...");
      bool success = vl53l0x_init();
      uint16_t ranges[3] = { 0 };
      while (success) {
          success = vl53l0x_read_range_single(VL53L0X_IDX_FIRST, &ranges[0]);
          ESP_LOGI(TAG, "Distancia: %d", ranges[0]);
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
