#ifndef _SERVO_INTERRUPTIONS_H_
#define _SERVO_INTERRUPTIONS_H_

#include <driver/gpio.h>
#include "servo.h"

#define LIMIT_SWITCH_PIN GPIO_NUM_5 // Revisar GPIO

esp_err_t interrupt_init(void);

#endif 