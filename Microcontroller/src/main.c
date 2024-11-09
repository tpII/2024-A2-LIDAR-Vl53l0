#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freeRtosTasks.h"
#include "servo.h"
#include "servo_interruptions.h"

static const char *TAG = "example";


void app_main(void)
{
 
  if(servo_initialize() != ESP_OK){
    ESP_LOGE(TAG,"ERROR Initialaizing servo");
    abort();
  }

  if(createTasks() != ESP_OK){
    ESP_LOGE(TAG,"ERROR Initialaizing Tasks");
    abort();
  }

  vTaskDelay(pdMS_TO_TICKS(3500));
  servo_start();

  while(1){
    /*servo_start();
    vTaskDelay(pdMS_TO_TICKS(1500));
    servo_stop();
    vTaskDelay(pdMS_TO_TICKS(2000));
    servo_set_speed(2100);
    vTaskDelay(pdMS_TO_TICKS(1500));
    servo_stop();*/
    //check_limit_switch();
    //vTaskDelay(pdMS_TO_TICKS(500));

  }
  
}


