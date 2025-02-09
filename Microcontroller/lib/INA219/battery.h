/**
 * @file mqtt_server.h
 * @author 
 *      Guerrico Leonel (lguerrico@outlook.com)
 * @brief Library for measuring battery level using the INA219 sensor.
 *
 * @version 1.0
 * @date 2025-02-09
 * 
 */

 

#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "esp_err.h"

/**
 * @brief Initializes the INA219 battery sensor.
 *
 * @return 
 *  - ESP_OK if initialization was successful.
 *  - ESP_ERR_INVALID_ARG if the configured shunt resistor value is invalid.
 *  - Another error code if sensor initialization fails.
 */
esp_err_t battery_sensor_init(void);

/**
 * @brief Reads the battery level as a percentage.
 *
 * @param[out] battery_level Pointer to a variable where the battery level (0-100%) will be stored.
 *
 * @return 
 *  - ESP_OK if reading was successful.
 *  - ESP_ERR_INVALID_ARG if the pointer is NULL.
 *  - Another error code if sensor reading fails.
 */
esp_err_t battery_sensor_read(uint8_t *);

#endif // _BATTERY_H_