#ifndef _LIMIT_SWITCH_H_
#define _LIMIT_SWITCH_H_

#include <driver/gpio.h>
#include "servo.h"

#define LIMIT_SWITCH_PIN GPIO_NUM_25 // Revisar GPIO

esp_err_t interrupt_init(void);

void check_limit_switch(void);

#endif 