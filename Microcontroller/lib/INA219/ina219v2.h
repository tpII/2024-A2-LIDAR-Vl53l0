/**
 * @file ina219v2.h
 * @author 
 *      Guerrico Leonel (lguerrico@outlook.com)
 * @brief Driver for INA219 current and power sensor (Limited adaptation).
 * 
 * This is a limited adaptation of the original library by UncleRus:
 * https://github.com/UncleRus/esp-idf-lib/blob/master/components/ina219/ina219.c
 *
 * @version 1.0
 * @date 2025-02-09
 */

#ifndef _INA219V2_H_
#define _INA219V2_H_

#include "esp_err.h"
#include "driver/i2c.h"

/** @brief Default I2C address of the INA219 sensor */
#define INA219_ADDRESS 0x40

/** @brief INA219 register addresses */
#define INA219_REG_CONFIG 0x00
#define INA219_REG_SHUNT_VOLTAGE 0x01
#define INA219_REG_BUS_VOLTAGE 0x02
#define INA219_REG_POWER 0x03
#define INA219_REG_CURRENT 0x04
#define INA219_REG_CALIBRATION 0x05

/** @brief Default configuration value for the INA219 */
#define INA219_CONFIG_DEFAULT 0x299F // Modo normal, rango de 16V, ADC 12 bits

/**
 * @brief INA219 device structure
 */
typedef struct {
    uint8_t i2c_addr; // Dirección I2C del dispositivo
} ina219_t;

/**
 * @brief Initializes the INA219 sensor.
 * 
 * @param dev Pointer to the INA219 device structure.
 * @return ESP_OK on success, ESP_FAIL on failure.
 */
esp_err_t ina219_init(ina219_t *dev);

/**
 * @brief Reads the bus voltage in volts.
 * 
 * @param dev Pointer to the INA219 device structure.
 * @param voltage Pointer to store the bus voltage (V).
 * @return ESP_OK on success, ESP_FAIL on failure.
 */
esp_err_t ina219_get_bus_voltage(ina219_t *dev, float *voltage);

/**
 * @brief Reads the shunt voltage in millivolts.
 * 
 * @param dev Pointer to the INA219 device structure.
 * @param voltage Pointer to store the shunt voltage (mV).
 * @return ESP_OK on success, ESP_FAIL on failure.
 */
esp_err_t ina219_get_shunt_voltage(ina219_t *dev, float *voltage);

/**
 * @brief Reads the current in milliamps.
 * 
 * @param dev Pointer to the INA219 device structure.
 * @param current Pointer to store the current (mA).
 * @return ESP_OK on success, ESP_FAIL on failure.
 */
esp_err_t ina219_get_current(ina219_t *dev, float *current);

/**
 * @brief Calibrates the INA219 for accurate current measurement.
 * 
 * @param dev Pointer to the INA219 device structure.
 * @param shunt_resistance The value of the shunt resistor in ohms.
 * @return ESP_OK on success, ESP_FAIL on failure.
 */

esp_err_t ina219_calibrate(ina219_t *dev, float shunt_resistance); // Configura la calibración

#endif // _INA219V2_H_
