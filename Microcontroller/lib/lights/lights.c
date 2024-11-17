#include "lights.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TIME 500

static volatile bool active = false;

esp_err_t lights_init(void)
{
    gpio_config_t io_conf;
    // Configurar los pines de los LEDs como salidas
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << ERROR_LED);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    if (gpio_config(&io_conf) != ESP_OK)
    {
        return ESP_FAIL;
    }

    // Asegurar que el LED esté apagado al inicio (nivel bajo)
    if (gpio_set_level(ERROR_LED, 0) != ESP_OK)
    {
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t error_led_on()
{
    return gpio_set_level(ERROR_LED, 1);
}

esp_err_t error_led_off()
{
    return gpio_set_level(ERROR_LED, 0);
}

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