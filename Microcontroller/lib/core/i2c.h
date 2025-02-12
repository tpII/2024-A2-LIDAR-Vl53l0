/**
 * @file i2c.h
 *
 * @brief I2C Master Interface Library for ESP32
 * 
 * This library provides functions for initializing and managing the I2C master interface
 * on the ESP32. It includes functionality to configure I2C pins, acquire exclusive bus
 * access, and manage communication timeouts.
 * 
 * @version 1.0
 * @date 2024-12-05
 * 
 * @author Guerrico Leonel (lguerrico@outlook.com)
 * 
 * @copyright Copyright (c) 2024 by Guerrico Leonel. All rights reserved.
 *
 */

#ifndef _I2C_H_
#define _I2C_H_

#include "esp_err.h"
#include "driver/i2c.h"
#include <stdbool.h>


#define I2C_MASTER_SCL_IO GPIO_NUM_22               ///< gpio number for I2C master clock
#define I2C_MASTER_SDA_IO GPIO_NUM_21               ///< gpio number for I2C master data 
#define I2C_MASTER_NUM I2C_NUM_0                    ///< I2C port number for master dev
#define I2C_MASTER_FREQ_HZ 400000                   ///< I2C master clock frequency
#define I2C_MASTER_TX_BUF_DISABLE 0                 ///< I2C master doesn't need buffer
#define I2C_MASTER_RX_BUF_DISABLE 0                 ///< I2C master doesn't need buffer
#define I2C_MASTER_TIMEOUT_MS 1000                  ///< I2C timeout in ms

/**
 * @brief Initializes the I2C master interface.
 *
 * This function configures the I2C master interface using predefined constants 
 * for GPIO pins, port number, and clock frequency. It must be called before
 * any other I2C operations.
 *
 * @return 
 *      - ESP_OK: I2C master was successfully initialized.
 *      - ESP_FAIL: Initialization failed.
 */
esp_err_t i2c_init(void);

/**
 * @brief Acquires the I2C bus for exclusive access.
 *
 * This function checks if the I2C bus is available and reserves it for use.
 * It prevents conflicts when multiple tasks attempt to use the I2C bus.
 *
 * @return 
 *      - `true`: If the bus was successfully acquired.
 *      - `false`: If the bus is already in use.
 */
bool i2c_get_bus();

/**
 * @brief Releases the I2C bus after use.
 *
 * This function signals that the I2C bus is no longer in use, making it available
 * for other tasks to access.
 *
 * @return 
 *      - `true`: If the bus was successfully released.
 *      - `false`: If the release operation failed.
 */
bool i2c_give_bus();

esp_err_t i2c_delete_bus();

#endif