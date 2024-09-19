#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "driver/gpio.h"

// Define los pines para los LEDs
#define ERROR_LED GPIO_NUM_2
#define POWER_LED GPIO_NUM_4

//Functions declarations

void lights_init(void);

void error_led_on(void);
void error_led_off(void);

void power_led_on(void);
void power_led_off(void);

void error_led_toggle(uint8_t frecunecy);

#endif