#ifndef _VL53L0X_H_
#define _VL53L0X_H_

// #include "esp_err.h"

// #define VL53L0X_ADDR 0x52
// #define VL53L0X_XSHUT 0
// #define VL53L0X_INT


// esp_err_t vl53l0x_init(void);

// esp_err_t vl53l0x_read(uint16_t *);

// esp_err_t vl53l0x_write(uint16_t);

// #endif

// Definición de la dirección I2C del VL53L0X
// #define VL53L0X_I2C_ADDRESS 0x29    //Dirección por default
// #define VL53L0X_I2C_HIGH_BYTE 0x1E
// #define VL53L0X_I2C_LOW_BYTE 0x1F


// Funciones públicas de la librería
// esp_err_t VL53L0X_start_ranging(void);
// uint16_t VL53L0X_read_range(void);
// esp_err_t VL53L0X_stop_ranging(void);




/*
    EXTRAÍDO DE https://github.com/artfulbytes/vl6180x_vl53l0x_msp430/tree/main
*/

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "esp_log.h"
#include "i2c.h"

typedef enum
{
    GPIO_XSHUT_FIRST,
    GPIO_XSHUT_SECOND,
    GPIO_XSHUT_THIRD,
} gpio_t;

typedef enum
{
    VL53L0X_IDX_FIRST,

    #ifdef VL53L0X_SECOND
        VL53L0X_IDX_SECOND,
    #endif

    #ifdef VL53L0X_THIRD
        VL53L0X_IDX_THIRD,
    #endif

} vl53l0x_idx_t;

#define VL53L0X_OUT_OF_RANGE (8190)

/**
 * Initializes the sensors in the vl53l0x_idx_t enum.
 * @note Each sensor must have its XSHUT pin connected.
 */
bool vl53l0x_init(void);

/**
 * Does a single range measurement
 * @param idx selects specific sensor
 * @param range contains the measured range or VL53L0X_OUT_OF_RANGE
 *        if out of range.
 * @return True if success, False if error
 * @note   Polling-based
 */
bool vl53l0x_read_range_single(vl53l0x_idx_t idx, uint16_t *range);

#endif
