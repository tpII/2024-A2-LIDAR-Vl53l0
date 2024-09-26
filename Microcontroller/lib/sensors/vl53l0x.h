#ifndef _VL53L0X_H_
#define _VL53L0X_H_

#include "esp_err.h"

#define VL53L0X_ADDR 0x52
#define VL53L0X_XSHUT 0
#define VL53L0X_INT


esp_err_t vl53l0x_init(void);

esp_err_t vl53l0x_read(uint16_t *);

esp_err_t vl53l0x_write(uint16_t);

#endif