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
TaskHandle_t ledBlinkTaskHandler = NULL;
BaseType_t task;
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
    if(task != NULL){
        return ESP_ERR_NOT_ALLOWED;
    }
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
    if(task != NULL){
        return ESP_ERR_NOT_ALLOWED;
    }
    return gpio_set_level(ERROR_LED, 0);
}

/**
 * @brief Task to blink the error LED.
 *
 * Alternates the error LED between on and off states with a delay of 500ms.
 * The blinking runs in an infinite loop, controlled by the `active` flag.
 * Ensures the LED turns off when the task stops.
 */

esp_err_t led_blink(uint16_t delay_ms)
{
    if (ledBlinkTaskHandler == NULL)
    {
        if (delay_ms == 0) // Si el delay es 0, no hace nada
        {
            return ESP_ERR_INVALID_ARG;
        }

        // Reservar memoria para pasar el valor a la tarea
        uint16_t *param = malloc(sizeof(uint16_t));
        if (param == NULL)
        {
            return ESP_FAIL;
        }
        *param = delay_ms;

        if (xTaskCreate(led_blink_task, "LedBlinkTask", 2048, param, 4, &ledBlinkTaskHandler) != pdPASS)
        {
            free(param); // Liberar memoria si la tarea no se pudo crear
            return ESP_FAIL;
        }
        return ESP_OK;
    }
    else
    {
        vTaskDelete(ledBlinkTaskHandler);
        ledBlinkTaskHandler = NULL;   // Limpiar el manejador de la tarea
        gpio_set_level(ERROR_LED, 0); // Apagar el LED
        return ESP_OK;
    }
}

void led_blink_task(void *parameter)
{

    uint16_t delay_ms = *((uint16_t *)parameter);
    free(parameter); // Liberar memoria reservada

    while (1)
    {
        if (gpio_set_level(ERROR_LED, 1) != ESP_OK)
        {
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));

        if (gpio_set_level(ERROR_LED, 0) != ESP_OK)
        {
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }

    vTaskDelete(NULL); // Eliminar la tarea cuando termi
}