#include "lights.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BLINK_FREQ  500 

void lights_init(void){
    gpio_config_t io_conf;
    // Configurar los pines de los LEDs como salidas
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << ERROR_LED);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    // Asegurar que el LED esté apagado al inicio (nivel bajo)
    gpio_set_level(ERROR_LED, 0);
}

void error_led_on(){
    gpio_set_level(ERROR_LED, 1);
}

void error_led_off(){
    gpio_set_level(ERROR_LED, 0);
}

void led_blink_task(void *pvParameter) {

    while (1) {
        // Cambiar el estado del LED
        gpio_set_level(ERROR_LED, 1);  // Encender
        vTaskDelay(pdMS_TO_TICKS(BLINK_FREQ));  // Esperar la mitad del periodo

        gpio_set_level(ERROR_LED, 0);  // Apagar
        vTaskDelay(pdMS_TO_TICKS(BLINK_FREQ));  // Esperar la mitad del periodo
    }
}