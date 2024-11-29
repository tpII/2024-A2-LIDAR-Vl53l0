#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "esp_err.h"

// Define los pines para los LEDs
#define ERROR_LED GPIO_NUM_12

//Functions declarations
esp_err_t lights_init(void);

esp_err_t error_led_on(void);
esp_err_t error_led_off(void);

void led_blink_task();

#endif