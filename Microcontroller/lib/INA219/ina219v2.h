#ifndef _INA219V2_H_
#define _INA219V2_H_

#include "esp_err.h"
#include "driver/i2c.h"

// Dirección I2C por defecto del INA219 (se puede modificar dependiendo del hardware)
#define INA219_ADDRESS 0x40

// Direcciones de los registros del INA219
#define INA219_REG_CONFIG 0x00
#define INA219_REG_SHUNT_VOLTAGE 0x01
#define INA219_REG_BUS_VOLTAGE 0x02
#define INA219_REG_POWER 0x03
#define INA219_REG_CURRENT 0x04
#define INA219_REG_CALIBRATION 0x05

// Configuración predeterminada del INA219
#define INA219_CONFIG_DEFAULT 0x399F // Modo normal, rango de 32V, ADC 12 bits

// Estructura para manejar el INA219
typedef struct {
    uint8_t i2c_addr; // Dirección I2C del dispositivo
} ina219_t;

// Declaraciones de funciones
esp_err_t ina219_init(ina219_t *dev); // Inicializa el INA219
esp_err_t ina219_get_bus_voltage(ina219_t *dev, float *voltage); // Obtiene el voltaje del bus (V)
esp_err_t ina219_get_shunt_voltage(ina219_t *dev, float *voltage); // Obtiene el voltaje del shunt (mV)
esp_err_t ina219_get_current(ina219_t *dev, float *current); // Obtiene la corriente (mA)
esp_err_t ina219_calibrate(ina219_t *dev, float shunt_resistance); // Configura la calibración

#endif
