#ifndef _SERVO_H_
#define _SERVO_H_

#include "esp_err.h"

// Prototipos de funciones
esp_err_t servo_initialize(void);
esp_err_t servo_start(void);
esp_err_t servo_stop(void);
esp_err_t servo_set_speed(uint32_t duty);


#endif