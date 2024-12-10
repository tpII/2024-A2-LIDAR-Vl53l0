/**
 * @file lights.h
 * @brief LED control library for ESP32
 * 
 * This library provides functions to initialize and control LEDs on the ESP32.
 * It supports turning an error LED on or off and running a blinking task for the LED.
 * 
 * @version 1.0
 * @date 2024-12-05
 * 
 * @note
 * - The `ERROR_LED` GPIO pin must be configured properly for the hardware.
 * - Ensure `lights_init` is called before using any other functions in this library.
 * 
 * @author Guerrico Leonel (lguerrico@outlook.com)
 * 
 */

#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "esp_err.h"

// GPIO pin definition for the error LED
#define ERROR_LED GPIO_NUM_12 ///< GPIO pin assigned to the error LED

/**
 * @brief Initializes the GPIO pins for LED control.
 * 
 * Configures the GPIO pin assigned to the error LED as an output. Ensures the LED 
 * is turned off initially.
 * 
 * @return 
 * - `ESP_OK`: If the GPIO configuration was successful.
 * - `ESP_FAIL`: If the configuration or initial setup fails.
 */
esp_err_t lights_init(void);

/**
 * @brief Turns on the error LED.
 * 
 * Sets the GPIO level for the error LED to high, turning it on.
 * 
 * @return 
 * - `ESP_OK`: If the operation was successful.
 * - `ESP_FAIL`: If the GPIO operation fails.
 */
esp_err_t error_led_on(void);

/**
 * @brief Turns off the error LED.
 * 
 * Sets the GPIO level for the error LED to low, turning it off.
 * 
 * @return 
 * - `ESP_OK`: If the operation was successful.
 * - `ESP_FAIL`: If the GPIO operation fails.
 */
esp_err_t error_led_off(void);

/**
 * @brief Task to blink the error LED.
 * 
 * This function runs a task that alternates the error LED between on and off 
 * states with a delay of 500ms. The blinking continues indefinitely or until 
 * stopped by external control.
 * 
 * @note
 * - The task must be created with FreeRTOS task management.
 */
void led_blink_task();

#endif