/**
 * @file mqtt_handler.h
 * @author Guerrico Leonel (lguerrico@outlook.com)
 * @brief Main functions for interacting with the MQTT service
 * 
 * This file contains the functions for sending data, and obtaining incoming instructions.
 * 
 * @version 1.0
 * @date 2024-12-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _MQTT_HANDLER_H_
#define _MQTT_HANDLER_H_

#include "esp_err.h"

/** 
  * @brief Retur by reference a instruction to execute
  * 
  * This function return a instruction to execute, by reference.
  * 
  * @param inst Instruction to be execute
  * @return
  *     - ESP_OK on success
  *     - ESP_FAIL on fail
*/
esp_err_t getInstruccionMessage(char *inst);

/**
 * @brief Send Mapping Value tuple
 * 
 * @param distance Distance
 * @param angle Angle
 * @return 
 *      - ESP_OK on success
 *      - ESP_FAIL on fail
 */
esp_err_t sendMappingValue(const uint16_t distance, const uint16_t angle);

/**
 * @brief Sends the battery charge percentage
 * 
 * @param batteryLevel Battery charge percentege f
 * @return 
 *      - ESP_OK on success
 *      - ESP_FAIL on fail
 */
esp_err_t sendBatteryLevel(const uint8_t batteryLevel);

/**
 * @brief Send Error Message 
 * 
 * @param TAG Library Tag
 * @param msg Message to send
 * @return 
 *      - ESP_OK on success 
 *      - ESP_FAIL on fail
 */
esp_err_t sendErrorMesage(const char *TAG, const char *msg);

/**
 * @brief Send Info Message 
 * 
 * @param TAG Library Tag
 * @param msg Message to send
 * @return 
 *      - ESP_OK on success 
 *      - ESP_FAIL on fail
 */
esp_err_t sendInfoMesage(const char *TAG, const char *msg);

/**
 * @brief Send Warning Message 
 * 
 * @param TAG Library Tag
 * @param msg Message to send
 * @return 
 *      - ESP_OK on success 
 *      - ESP_FAIL on fail
 */
esp_err_t sendWarningMesage(const char *TAG, const char *msg);

#endif //HTTP_HANDLER_H_
