#include "lights.h"

void lights_init(void){
    gpio_config_t io_conf;
    // Configurar los pines de los LEDs como salidas
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << ERROR_LED) | (1ULL << POWER_LED);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

void error_led_on(void){
    gpio_set_level(ERROR_LED, 1);
}

void error_led_off(void){
    gpio_set_level(ERROR_LED, 0);
}

void power_led_on(void){
    gpio_set_level(POWER_LED, 1);
}

void power_led_off(void){
    gpio_set_level(POWER_LED, 0);
}

//Reacer la secuencia de encendido y apagado del LED de error
void error_led_toggle(uint8_t frecuency){
    for(int i = 0; i < frecuency; i++){
        error_led_on();
        vTaskDelay(500 / portTICK_PERIOD_MS);
        error_led_off();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}