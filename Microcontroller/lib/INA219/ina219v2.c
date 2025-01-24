#include "ina219v2.h"
#include "i2c.h"
#include "esp_log.h"

#define TAG "INA219"

static esp_err_t ina219_write_register(ina219_t *, uint8_t, uint16_t);  // Escribe un registro
static esp_err_t ina219_read_register(ina219_t *, uint8_t, uint16_t *); // Lee un registro

// Inicialización del INA219
esp_err_t ina219_init(ina219_t *dev)
{
    if (dev == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }

    if (i2c_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR I2C INTI");
        return ESP_FAIL;
    }

    // Configuración por defecto del INA219
    return ina219_write_register(dev, INA219_REG_CONFIG, INA219_CONFIG_DEFAULT);
}

// Escribir en un registro
static esp_err_t ina219_write_register(ina219_t *dev, uint8_t reg, uint16_t value)
{
    ESP_LOGI(TAG, "Enviando a dirección: 0x%02X, Registro: 0x%02X, MSB: 0x%02X, LSB: 0x%02X",
             dev->i2c_addr, reg, (value >> 8) & 0xFF, value & 0xFF);

    ESP_LOGI(TAG, "Obteniendo bus");

    if (!i2c_get_bus())
    {
        ESP_LOGE(TAG, "BUS BUSY");
        return ESP_FAIL; // No se pudo obtener el bus
    }

    ESP_LOGI(TAG, "Bus obtenido");

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (!cmd)
    {
        ESP_LOGE(TAG, "CMD NULL");
        i2c_give_bus();
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(TAG, "CMD CREADO");

    esp_err_t ret = ESP_OK;
    uint8_t data[] = {reg, (value >> 8) & 0xFF, value & 0xFF};

    ret = i2c_master_start(cmd);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error en i2c_master_start: %s", esp_err_to_name(ret));
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ret;
    }
    // Escribir dirección del dispositivo con bit de escritura
    ret = i2c_master_write_byte(cmd, (dev->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error enviando dirección: %s", esp_err_to_name(ret));
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ret;
    }

    // Escribir registro y datos
    ret = i2c_master_write(cmd, data, sizeof(data), true);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error enviando datos: %s", esp_err_to_name(ret));
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ret;
    }

    // Finalizar comunicación I2C
    ret = i2c_master_stop(cmd);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error en i2c_master_stop: %s", esp_err_to_name(ret));
        i2c_cmd_link_delete(cmd);
        i2c_give_bus();
        return ret;
    }

    // Ejecutar los comandos en el bus
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error en i2c_master_cmd_begin: %s", esp_err_to_name(ret));
    }
    i2c_cmd_link_delete(cmd);
    i2c_give_bus();

    return ret;
}

// Leer un registro
static esp_err_t ina219_read_register(ina219_t *dev, uint8_t reg, uint16_t *value)
{
    if (value == NULL || !i2c_get_bus())
    {
        return ESP_ERR_INVALID_ARG;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (!cmd)
    {
        i2c_give_bus();
        return ESP_ERR_NO_MEM;
    }

    esp_err_t ret = ESP_OK;

    // Escritura del registro
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK)
    {
        i2c_give_bus();
        return ret;
    }

    // Lectura del valor
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev->i2c_addr << 1) | I2C_MASTER_READ, true);
    uint8_t data[2];
    i2c_master_read(cmd, data, sizeof(data), I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    i2c_give_bus();

    if (ret == ESP_OK)
    {
        *value = (data[0] << 8) | data[1];
    }

    return ret;
}

// Obtener el voltaje del bus
esp_err_t ina219_get_bus_voltage(ina219_t *dev, float *voltage)
{
    uint16_t value;
    esp_err_t ret = ina219_read_register(dev, INA219_REG_BUS_VOLTAGE, &value);
    if (ret == ESP_OK)
    {
        *voltage = (value >> 3) * 0.004; // Cada bit equivale a 4mV
    }
    return ret;
}

// Obtener el voltaje del shunt
esp_err_t ina219_get_shunt_voltage(ina219_t *dev, float *voltage)
{
    uint16_t value;
    esp_err_t ret = ina219_read_register(dev, INA219_REG_SHUNT_VOLTAGE, &value);
    if (ret == ESP_OK)
    {
        *voltage = (int16_t)value * 0.01; // Cada bit equivale a 10uV
    }
    return ret;
}

// Obtener la corriente (requiere calibración previa)
esp_err_t ina219_get_current(ina219_t *dev, float *current)
{
    uint16_t value;
    esp_err_t ret = ina219_read_register(dev, INA219_REG_CURRENT, &value);
    if (ret == ESP_OK)
    {
        *current = (int16_t)value * 0.01; // Depende de la calibración
    }
    return ret;
}

// Calibrar el INA219
esp_err_t ina219_calibrate(ina219_t *dev, float shunt_resistance)
{
    uint16_t calibration = (uint16_t)(0.04096 / (shunt_resistance * (0.00006))); // Ejemplo para 1mOhm
    return ina219_write_register(dev, INA219_REG_CALIBRATION, calibration);
}
