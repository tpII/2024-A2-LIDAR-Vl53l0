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
#include "lwip/sockets.h"

#define ESP_WIFI_SSID      "Cyclops"//CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS      "A2TdP2Lidar"//CONFIG_ESP_WIFI_PASSWORD
#define ESP_WIFI_CHANNEL   1//CONFIG_ESP_WIFI_CHANNEL
#define MAX_STA_CONN       4//CONFIG_ESP_MAX_STA_CONN

#define PORT 7777
#define KEEPALIVE_IDLE              5 // TCP keep-alive idle time. In idle time without receiving any data from peer, will send keep-alive probe packet
#define KEEPALIVE_INTERVAL          5 // TCP keep-alive interval time
#define KEEPALIVE_COUNT             3 // TCP keep-alive packet retry send counts

#define MAX_NUM_CONNECTIONS 1 //NERVER CHANGE THIS NUMBER

esp_err_t wifi_init_softap(void);
esp_err_t initialize_server(void);

#endif