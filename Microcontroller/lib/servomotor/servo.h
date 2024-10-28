#ifndef _SERVO_H_
#define _SERVO_H_

#include "esp_err.h"

// Inicializa el servo motor en un GPIO específico (predefinido)
esp_err_t servo_initialize(void);

// Establece el ángulo del servomotor
esp_err_t servo_write_angle(int angle);

// Lee el ángulo actual del servomotor
esp_err_t servo_read_angle(float *angle);

#endif