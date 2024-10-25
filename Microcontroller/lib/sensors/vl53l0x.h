// #ifndef _VL53L0X_H_
// #define _VL53L0X_H_

// #include "esp_err.h"

// #define VL53L0X_ADDR 0x52
// #define VL53L0X_XSHUT 0
// #define VL53L0X_INT


// esp_err_t vl53l0x_init(void);

// esp_err_t vl53l0x_read(uint16_t *);

// esp_err_t vl53l0x_write(uint16_t);

// #endif

#ifndef _VL53L0X_H_
#define _VL53L0X_H_

#include "driver/i2c.h"
#include "esp_err.h"
#include "i2c.h"

// Definición de la dirección I2C del VL53L0X
//#define VL53L0X_I2C_ADDRESS 0x29    //Dirección por default
#define VL53L0X_I2C_ADDRESS 0x52    //Dirección por default
#define VL53L0X_I2C_HIGH_BYTE 0x1E
#define VL53L0X_I2C_LOW_BYTE 0x1F

// #define VL53L0X_ADDR 0x52
// #define VL53L0X_XSHUT 0
// #define VL53L0X_INT

// Funciones públicas de la librería
esp_err_t VL53L0X_start_ranging(void);
uint16_t VL53L0X_read_range(void);
esp_err_t VL53L0X_stop_ranging(void);

#endif
