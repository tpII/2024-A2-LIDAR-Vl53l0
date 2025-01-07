/*
 * Copyright (c) 2019 Ruslan V. Uss <unclerus@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of itscontributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file ina219.c
 *
 * ESP-IDF driver for INA219/INA220 Zerø-Drift, Bidirectional
 * Current/Power Monitor
 *
 * Copyright (c) 2019 Ruslan V. Uss <unclerus@gmail.com>
 *
 * BSD Licensed as described in the file LICENSE
 */

#include "ina219.h"
#include "esp_log.h"
#include <math.h>
// #include "esp_idf_lib_helpers.h"

#define REG_CONFIG 0
#define REG_SHUNT_U 1
#define REG_BUS_U 2
#define REG_POWER 3
#define REG_CURRENT 4
#define REG_CALIBRATION 5

#define BIT_RST 15
#define BIT_BRNG 13
#define BIT_PG0 11
#define BIT_BADC0 7
#define BIT_SADC0 3
#define BIT_MODE 0

#define MASK_PG (3 << BIT_PG0)
#define MASK_BADC (0xf << BIT_BADC0)
#define MASK_SADC (0xf << BIT_SADC0)
#define MASK_MODE (7 << BIT_MODE)
#define MASK_BRNG (1 << BIT_BRNG)

#define DEF_CONFIG 0x399f

#define CHECK(x)                \
    do                          \
    {                           \
        esp_err_t __;           \
        if ((__ = x) != ESP_OK) \
            return __;          \
    } while (0)
#define CHECK_ARG(VAL)                  \
    do                                  \
    {                                   \
        if (!(VAL))                     \
            return ESP_ERR_INVALID_ARG; \
    } while (0)

static const char *TAG = "INA219";

static const float u_shunt_max[] = {
    [INA219_GAIN_1] = 0.04,
    [INA219_GAIN_0_5] = 0.08,
    [INA219_GAIN_0_25] = 0.16,
    [INA219_GAIN_0_125] = 0.32,
};

static esp_err_t read_reg_16(ina219_t *dev, uint8_t reg_addr, uint16_t *val)
{
    CHECK_ARG(val);

    if (!i2c_get_bus())
    {
        ESP_LOGE(TAG, "Error obteniendo el bus I2C");
        return ESP_FAIL;
    }

    // Crear un comando I2C
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL)
    {
        ESP_LOGE(TAG, "Error al crear el comando I2C");
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Iniciar la transferencia en modo escritura
    i2c_master_start(cmd);

    // Escribir la dirección del dispositivo en el bus (modo escritura)
    if (i2c_master_write_byte(cmd, (dev->addr << 1) | I2C_MASTER_WRITE, true) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al escribir la dirección del dispositivo");
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Escribir el registro que queremos leer
    if (i2c_master_write_byte(cmd, reg_addr, true) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al escribir el registro");
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Iniciar la transferencia para cambiar a modo lectura
    i2c_master_start(cmd);

    // Escribir la dirección del dispositivo en modo lectura
    if (i2c_master_write_byte(cmd, (dev->addr << 1) | I2C_MASTER_READ, true) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al escribir la dirección del dispositivo en modo lectura");
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Leer los 2 bytes (16 bits) en el buffer
    if (i2c_master_read_byte(cmd, (uint8_t *)(val + 1), I2C_MASTER_ACK) != ESP_OK ||
        i2c_master_read_byte(cmd, (uint8_t *)(val), I2C_MASTER_LAST_NACK) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error en la lectura de 16 bits");
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Finalizar la transferencia I2C
    if (i2c_master_stop(cmd) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al finalizar la transferencia");
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Ejecutar el comando
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error en la transferencia I2C: %s", esp_err_to_name(ret));
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Liberar el comando I2C
    i2c_cmd_link_delete(cmd);

    // Liberar el bus I2C
    if (!i2c_give_bus())
    {
        return ESP_FAIL;
    }

    // Intercambiar los bytes para obtener el valor correcto (Big Endian a Little Endian)
    *val = (*val >> 8) | (*val << 8);

    return ESP_OK;
}

static esp_err_t write_reg_16(ina219_t *dev, uint8_t reg_addr, uint16_t val)
{
    uint16_t value_to_write = (val >> 8) | (val << 8);

    if (!i2c_get_bus())
    {
        ESP_LOGE(TAG, "Error obteniendo el bus I2C");
        return ESP_FAIL;
    }

    // Crear un comando I2C
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL)
    {
        ESP_LOGE(TAG, "Error al crear comando I2C");
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Iniciar la transferencia I2C
    i2c_master_start(cmd);

    // Escribir la dirección del dispositivo en modo escritura
    if (i2c_master_write_byte(cmd, (dev->addr << 1) | I2C_MASTER_WRITE, true) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al escribir la dirección del dispositivo");
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Escribir la dirección del registro
    if (i2c_master_write_byte(cmd, reg_addr, true) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al escribir el registro");
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Escribir los dos bytes del valor (MSB primero)
    if (i2c_master_write_byte(cmd, (value_to_write >> 8) & 0xFF, true) != ESP_OK ||
        i2c_master_write_byte(cmd, value_to_write & 0xFF, true) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al escribir el valor de 16 bits");
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Finalizar la transferencia
    if (i2c_master_stop(cmd) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al finalizar la transferencia I2C");
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ESP_FAIL;
    }

    // Enviar el comando y verificar el resultado
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error en la transferencia I2C: %s", esp_err_to_name(ret));
    }

    // Liberar el comando y el bus I2C
    i2c_cmd_link_delete(cmd);
    if (!i2c_give_bus())
    {
        return ESP_FAIL;
    }

    return ret;
}

static esp_err_t read_conf_bits(ina219_t *dev, uint16_t mask, uint8_t bit, uint16_t *res)
{
    uint16_t raw;
    CHECK(read_reg_16(dev, REG_CONFIG, &raw));

    *res = (raw & mask) >> bit;

    return ESP_OK;
}

///////////////////////////////////////////////////////////////////////////////


esp_err_t ina219_init(ina219_t *dev)
{

    CHECK_ARG(dev);

    // Leer el valor del registro de configuración
    CHECK(read_reg_16(dev, REG_CONFIG, &dev->config));

    // Registrar la configuración leída para depuración
    ESP_LOGD(TAG, "Initialize, config: 0x%04x", dev->config);

    return ESP_OK;
}

esp_err_t ina219_reset(ina219_t *dev)
{

    CHECK_ARG(dev);
    CHECK(write_reg_16(dev, REG_CONFIG, 1 << BIT_RST));

    dev->config = DEF_CONFIG;

    ESP_LOGD(TAG, "Device reset");

    return ESP_OK;
}

esp_err_t ina219_configure(ina219_t *dev, ina219_bus_voltage_range_t u_range,
                           ina219_gain_t gain, ina219_resolution_t u_res,
                           ina219_resolution_t i_res, ina219_mode_t mode)
{
    CHECK_ARG(dev);
    CHECK_ARG(u_range <= INA219_BUS_RANGE_32V);
    CHECK_ARG(gain <= INA219_GAIN_0_125);
    CHECK_ARG(u_res <= INA219_RES_12BIT_128S);
    CHECK_ARG(i_res <= INA219_RES_12BIT_128S);
    CHECK_ARG(mode <= INA219_MODE_CONT_SHUNT_BUS);

    dev->config = (u_range << BIT_BRNG) |
                  (gain << BIT_PG0)     |
                  (u_res << BIT_BADC0)  |
                  (i_res << BIT_SADC0)  |
                  (mode << BIT_MODE);

    ESP_LOGD(TAG, "Config: 0x%04x", dev->config);

    return write_reg_16(dev, REG_CONFIG, dev->config);
}

esp_err_t ina219_get_bus_voltage_range(ina219_t *dev, ina219_bus_voltage_range_t *range)
{
    // Verificar que el puntero a range no sea NULL
    CHECK_ARG(dev && range);
    *range = 0;

    return read_conf_bits(dev, MASK_BRNG, BIT_BRNG, (uint16_t *)range);
}

esp_err_t ina219_get_gain(ina219_t *dev, ina219_gain_t *gain)
{
    CHECK_ARG(dev && gain);
    *gain = 0;
    return read_conf_bits(dev, MASK_PG, BIT_PG0, (uint16_t *)gain);
}

esp_err_t ina219_get_bus_voltage_resolution(ina219_t *dev, ina219_resolution_t *res)
{
    CHECK_ARG(dev && res);
    *res = 0;
    return read_conf_bits(dev, MASK_BADC, BIT_BADC0, (uint16_t *)res);
}

esp_err_t ina219_get_shunt_voltage_resolution(ina219_t *dev, ina219_resolution_t *res)
{
    CHECK_ARG(dev && res);
    *res = 0;
    return read_conf_bits(dev, MASK_SADC, BIT_SADC0, (uint16_t *)res);
}

esp_err_t ina219_get_mode(ina219_t *dev, ina219_mode_t *mode)
{
    CHECK_ARG(dev && mode);
    *mode = 0;
    return read_conf_bits(dev, MASK_MODE, BIT_MODE, (uint16_t *)mode);
}

esp_err_t ina219_calibrate(ina219_t *dev, float r_shunt)
{

    CHECK_ARG(dev);

    ina219_gain_t gain;
    CHECK(ina219_get_gain(dev, &gain));
  
    dev->i_lsb = (uint16_t)(u_shunt_max[gain] / r_shunt / 32767 * 100000000);
    dev->i_lsb /= 100000000;
    dev->i_lsb /= 0.0001;
    dev->i_lsb = ceil(dev->i_lsb);
    dev->i_lsb *= 0.0001;

    dev->p_lsb = dev->i_lsb * 20;

    uint16_t cal = (uint16_t)((0.04096) / (dev->i_lsb * r_shunt));

    ESP_LOGD(TAG, "Calibration: %.04f Ohm, 0x%04x", r_shunt, cal);

    return write_reg_16(dev, REG_CALIBRATION, cal);
}

esp_err_t ina219_trigger(ina219_t *dev)
{
    CHECK_ARG(dev);

    uint16_t mode = (dev->config & MASK_MODE) >> BIT_MODE;
    if (mode < INA219_MODE_TRIG_SHUNT || mode > INA219_MODE_TRIG_SHUNT_BUS)
    {
        ESP_LOGE(TAG, "Could not trigger conversion in this mode: %d", mode);
        return ESP_ERR_INVALID_STATE;
    }

    return write_reg_16(dev, REG_CONFIG, dev->config);
}

esp_err_t ina219_get_bus_voltage(ina219_t *dev, float *voltage)
{
    CHECK_ARG(dev && voltage);

    uint16_t raw;
    CHECK(read_reg_16(dev, REG_BUS_U, &raw));

    *voltage = (raw >> 3) * 0.004;

    return ESP_OK;
}

esp_err_t ina219_get_shunt_voltage(ina219_t *dev, float *voltage)
{
    CHECK_ARG(dev && voltage);

    int16_t raw;
    CHECK(read_reg_16(dev, REG_SHUNT_U, (uint16_t *)&raw));

    *voltage = raw / 100000.0;

    return ESP_OK;
}

esp_err_t ina219_get_current(ina219_t *dev, float *current)
{
    CHECK_ARG(dev && current);

    int16_t raw;
    CHECK(read_reg_16(dev, REG_CURRENT, (uint16_t *)&raw));

    *current = raw * dev->i_lsb;

    return ESP_OK;
}

esp_err_t ina219_get_power(ina219_t *dev, float *power)
{
    CHECK_ARG(dev && power);

    int16_t raw;
    CHECK(read_reg_16(dev, REG_POWER, (uint16_t *)&raw));

    *power = raw * dev->p_lsb;

    return ESP_OK;
}