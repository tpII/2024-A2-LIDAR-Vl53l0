#include "esp_log.h"
#include "ap_server.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_server.h"

#include "mqtt_handler.h"


#include <stdio.h>
#include <string.h>

static const char *TAG = "MAIN";

esp_err_t initialize()
{
  esp_err_t err2 = initialize_server();
  if (err2)
  {
    ESP_LOGE(TAG, "Error ocurred trying to initialize server: err %d", err2);
    return ESP_FAIL;
  }

  while(wait_for_client_connection() != ESP_OK);
  vTaskDelay(pdMS_TO_TICKS(200));                 //ESPERO 200ms por seguridad
  mqtt_start();

  return ESP_OK;
}

void task()
{
  int id = 0;
  char base_msg [] = "MCU - BACK => Test Message Nro: ";
  char final_msg[100];
  char id_str[10];
  char inst[40];
  while (id < 100)
  {
    
    /*//Testing SendControl Message
    strcpy(final_msg,base_msg);
    sprintf(id_str,"%d",id);
    strcat(final_msg,id_str);
    ESP_LOGI(TAG,"Message to Send: %s",final_msg);
    ESP_LOGI(TAG,"Size of message to send: %u",strlen(final_msg));
    esp_err_t err = sendControlMessage(final_msg,strlen(final_msg));
    if(err != ESP_OK) ESP_LOGE(TAG,"POST FAIL => %s",esp_err_to_name(err));
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    //Testing SendSensorValue
    ESP_LOGI(TAG,"Sensor Value to Send: %d",id);
    esp_err_t err2 = sendMappingValue(id,id);
    if(err2 != ESP_OK) ESP_LOGE(TAG,"POST FAIL => %s",esp_err_to_name(err2));
    vTaskDelay(pdMS_TO_TICKS(5000));
    */
    ESP_LOGI(TAG,"Iteration Nro: %d",id);
    esp_err_t err3 = getInstruccionMessage(inst);
    if(err3 != ESP_OK){
      ESP_LOGW(TAG,"Not Instruction");
    }else {
      ESP_LOGI(TAG,"Instructtion obtained: %s",inst);
    }
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
  vTaskDelay(pdMS_TO_TICKS(5000));
  task();
}