#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "servo.h"

#include "driver/mcpwm_prelude.h"

static const char *TAG = "example";


void app_main(void)
{
 
  if(servo_initialize() != ESP_OK){
    ESP_LOGE(TAG,"ERROR Initialaizing servo");
    abort();
  }

  vTaskDelay(pdMS_TO_TICKS(3500));

  while(1){
    servo_start();
    vTaskDelay(pdMS_TO_TICKS(2000));
    servo_stop();
    vTaskDelay(pdMS_TO_TICKS(2000));
    servo_set_speed(2100);
    vTaskDelay(pdMS_TO_TICKS(2000));
    servo_stop();
  }
  
}


