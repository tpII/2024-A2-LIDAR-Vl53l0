#ifndef _LIGHTS_H_
#define _LIGHTS_H_



// Define los pines para los LEDs
#define ERROR_LED GPIO_NUM_12

//Functions declarations
void lights_init(void);

void error_init(void);

void error_led_on(void);
void error_led_off(void);

void led_blink_task(void *pvParameter);

#endif