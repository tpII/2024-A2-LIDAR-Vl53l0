#include "http_handler.h"
#include "esp_log.h"

#include "ap_server.h"
#include "http_server.h"

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *TAG = "MAIN";

esp_err_t initialize()
{
  esp_err_t err2 = initialize_server();
  if (err2)
  {
    ESP_LOGE(TAG, "Error ocurred trying to initialize server: err %d", err2);
    return ESP_FAIL;
  }

  start_http_server();

  return ESP_OK;
}

void task()
{
  int id = 0;
  char base_msg [] = "MCU - BACK => Test Message Nro: ";
  char final_msg[100];
  char id_str[10];
  while (id < 255)
  {
    strcpy(final_msg,base_msg);
    sprintf(id_str,"%d",id);
    strcat(final_msg,id_str);
    ESP_LOGI(TAG,"Message to Send: %s",final_msg);
    ESP_LOGI(TAG,"Size of message to send: %u",(unsigned int)sizeof(final_msg));
    esp_err_t err = sendControlMessage(final_msg,sizeof(final_msg));
    if(err != ESP_OK) ESP_LOGE(TAG,"POST FAIL => %s",esp_err_to_name(err));
    vTaskDelay(pdMS_TO_TICKS(5000));
    id++;
  }
}

void app_main(void)
{

  esp_err_t err = initialize();
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG,"INITIALIZE FAIL");
    while(1);
  }
  ESP_LOGI(TAG,"INITIALIZE SUCCESS");
  task();
}