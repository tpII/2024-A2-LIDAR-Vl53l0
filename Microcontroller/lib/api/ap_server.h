#ifndef _AP_SERVER_H_
#define _AP_SERVER_H_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <portmacro.h>
#include "lwip/err.h"
#include "lwip/sys.h"

#define ESP_WIFI_SSID      "Cyclops"//CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS      "A2TdP2"//CONFIG_ESP_WIFI_PASSWORD
#define ESP_WIFI_CHANNEL   1//CONFIG_ESP_WIFI_CHANNEL
#define MAX_STA_CONN       4//CONFIG_ESP_MAX_STA_CONN

void vTask1(void *pvParameters);  //funcion de prueba para freeRTOS
static void wifi_event_handler(void*, esp_event_base_t , int32_t , void* );
void wifi_init_softap(void);
#endif