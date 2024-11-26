#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freeRtosTasks.h"
#include "servo.h"
#include "servo_interruptions.h"
#include <rom/ets_sys.h>
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
    ESP_LOGW(TAG,"ANGLE READED: %d",readAngle());
    vTaskDelay(5 / portTICK_PERIOD_MS);
    //ets_delay_us(100);


  }
  
}


