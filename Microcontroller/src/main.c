#include "esp_log.h"
#include "ap_server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_server.h"

#include "mqtt_handler.h"
#include "lights.h"
#include <stdio.h>
#include <string.h>

static const char *TAG = "MAIN";

esp_err_t initialize()
{
  if(lights_init() != ESP_OK){
    return ESP_FAIL;
  }
  /*esp_err_t err2 = initialize_server();
  if (err2)
  {
    ESP_LOGE(TAG, "Error ocurred trying to initialize server: err %d", err2);
    return ESP_FAIL;
  }

  while (wait_for_client_connection() != ESP_OK)
    ;
  vTaskDelay(pdMS_TO_TICKS(200)); // ESPERO 200ms por seguridad
  mqtt_start();
  */

  return ESP_OK;
}


void app_main(void)
{

  esp_err_t err = initialize();
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "INITIALIZE FAIL");
    while (1)
      ;
  }
  ESP_LOGI(TAG, "INITIALIZE SUCCESS");
  vTaskDelay(pdMS_TO_TICKS(2000));
  led_blink_task();
  /*while(1){

    if(error_led_on() != ESP_OK){
      ESP_LOGE(TAG,"ERROR TURNING ON LED");
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
    if(error_led_off() != ESP_OK){
      ESP_LOGE(TAG,"ERROR TURNING OFF LED");
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }*/
}