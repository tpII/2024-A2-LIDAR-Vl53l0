#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "esp_err.h"

esp_err_t battery_sensor_init(void);

esp_err_t battery_sensor_read(uint8_t *);

#endif