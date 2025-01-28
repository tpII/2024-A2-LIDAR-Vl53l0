/**
 * @file ap_server.h
 * @author 
 *      Ossola Florencia (flor.ossola13@gmail.com)
 * @brief Header file for initializing and configuring the WiFi SoftAP server.
 * 
 * This header contains the definitions and function declarations necessary for 
 * setting up a WiFi SoftAP on an ESP32 device. It includes configurations for WiFi
 * and connection handling.
 * 
 * This implementation is partially based on examples provided by the ESP-IDF 
 * library, which have been customized to meet the requirements of this project.
 * 
 * 
 * @version 1.0
 * @date 2024-12-09
 * 
 * @copyright
 * Copyright (c) 2024 by Ossola Florencia. All rights reserved.
 */
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

#define ESP_WIFI_SSID      "Cyclops"//CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS      "A2TdP2Lidar"//CONFIG_ESP_WIFI_PASSWORD
#define ESP_WIFI_CHANNEL   1//CONFIG_ESP_WIFI_CHANNEL
#define MAX_STA_CONN       4//CONFIG_ESP_MAX_STA_CONN

#define PORT 7777
#define MAX_NUM_CONNECTIONS 1 //NERVER CHANGE THIS NUMBER


/**
 * @brief Initialize the SoftAP Wi-Fi functionality.
 * 
 * This function initializes the Wi-Fi access point (AP) functionality for the ESP32. It 
 * sets up the necessary configurations, registers event handlers, and starts the Wi-Fi 
 * interface. Additionally, it creates a binary semaphore for client connection management.
 * The AP is configured with a specified SSID, password, and channel, and will start
 * accepting connections.
 * 
 * @param[in] None
 * 
 * @return
 *      - ESP_OK on success
 *      - ESP_FAIL on failure
 */
esp_err_t wifi_init_softap(void);


/**
 * @brief Initialize the server and Wi-Fi SoftAP functionality.
 * 
 * This function initializes the NVS (Non-Volatile Storage) and sets up the Wi-Fi SoftAP.
 * If there are any issues with NVS (such as no free pages or a new version found), it erases 
 * the storage and reinitializes it. After that, it initializes the SoftAP Wi-Fi. The Wi-Fi 
 * SoftAP mode is configured, and an error is showed if the initialization fails.
 * 
 * @param[in] None
 * 
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_NVS_NO_FREE_PAGES if NVS has no free pages
 *      - ESP_ERR_NVS_NEW_VERSION_FOUND if a new version of NVS is found
 *      - ESP_FAIL on failure in other parts of the initialization
 */
esp_err_t initialize_server(void);


/**
 * @brief Wait for connection with a client
 * 
 * This function blocks the execution until a device successfully connects to the SoftAP. 
 * It uses a binary semaphore to synchronize the connection process. Once the semaphore is 
 * released (indicating that a device is connected), the function shows a message and continues.
 * 
 * @param[in] none
 * @return
 *      - ESP_OK on success
 */
esp_err_t wait_for_client_connection(void); 

esp_err_t delete_client_semaphore(void);

#endif