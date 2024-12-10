/**
 * @file lights.c
 * @brief Implementation of LED control library for ESP32
 * 
 * This file implements the functions for initializing and controlling an error LED. 
 * It includes basic GPIO operations to turn the LED on, off, or make it blink 
 * periodically in a FreeRTOS task.
 * 
 * @version 1.0
 * @date 2024-12-05
 * 
 * @note
 * - Ensure the `ERROR_LED` pin is connected correctly to the hardware.
 * - Call `lights_init` before invoking other functions.
 * - Use FreeRTOS to manage `led_blink_task`.
 * 
 * @author Guerrico Leonel (lguerrico@outlook.com)
 * 
 */


#include "lights.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Constants
#define TIME 500 ///< Blinking interval in milliseconds

// Global Variables
static volatile bool active = false;

/**
 * @brief Initializes the GPIO pins for LED control.
 * 
 * Configures the GPIO pin for the error LED as an output. Ensures the LED 
 * starts in the off state (low level).
 * 
 * @return 
 * - `ESP_OK`: If the GPIO initialization was successful.
 * - `ESP_FAIL`: If the GPIO configuration or initial setup fails.
 */
esp_err_t lights_init(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << ERROR_LED);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    if (gpio_config(&io_conf) != ESP_OK)
    {
        return ESP_FAIL;
    }

    if (gpio_set_level(ERROR_LED, 0) != ESP_OK)
    {
        return ESP_FAIL;
    }

    return ESP_OK;
}

/**
 * @brief Turns on the error LED.
 * 
 * Sets the GPIO level for the error LED to high, turning it on.
 * 
 * @return 
 * - `ESP_OK`: If the operation was successful.
 * - `ESP_FAIL`: If the GPIO operation fails.
 */
esp_err_t error_led_on()
{
    return gpio_set_level(ERROR_LED, 1);
}

/**
 * @brief Turns off the error LED.
 * 
 * Sets the GPIO level for the error LED to low, turning it off.
 * 
 * @return 
 * - `ESP_OK`: If the operation was successful.
 * - `ESP_FAIL`: If the GPIO operation fails.
 */
esp_err_t error_led_off()
{
    return gpio_set_level(ERROR_LED, 0);
}

/**
 * @brief Task to blink the error LED.
 * 
 * Alternates the error LED between on and off states with a delay of 500ms. 
 * The blinking runs in an infinite loop, controlled by the `active` flag. 
 * Ensures the LED turns off when the task stops.
 */
void led_blink_task()
{
    if (!active)
    {
        while (1)
        {

            if (gpio_set_level(ERROR_LED, 1) != ESP_OK)
            {
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(TIME));
            if (gpio_set_level(ERROR_LED, 0) != ESP_OK)
            {
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(TIME));
        }
    }
    active = !active;
    gpio_set_level(ERROR_LED, 0); // Apagar
}