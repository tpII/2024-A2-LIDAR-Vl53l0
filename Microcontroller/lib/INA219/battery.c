#include "battery.h"
#include "esp_log.h"
#include "ina219v2.h"
#include "string.h"
#define CONFIG_SHUNT_RESISTOR_MILLI_OHM 100 // SHOULD BE ADDED DIRECT IN sdkconfig

#define FULL_CHARGE 8.4 //FULL CHARGE 8.4V
#define MIN_CHARGE  6.4 //MIN CHARGE 6V (0%)

static const char *TAG = "BATTERY";

static ina219_t dev;
static float bus_voltage;
static float shunt_voltage;
static float current;
static float power;

esp_err_t battery_sensor_init()
{

    memset(&dev, 0, sizeof(ina219_t));

    if (CONFIG_SHUNT_RESISTOR_MILLI_OHM <= 0)
    {
        ESP_LOGE(TAG, "Invalid shunt resistor value: %d mOhm", CONFIG_SHUNT_RESISTOR_MILLI_OHM);
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Initalizing INA219");
    ESP_ERROR_CHECK(ina219_init(&dev));

    ESP_LOGI(TAG, "Calibrating INA219");

    ESP_ERROR_CHECK(ina219_calibrate(&dev, (float)CONFIG_SHUNT_RESISTOR_MILLI_OHM / 1000.0f));
    
    bus_voltage = 0.0f;
    shunt_voltage = 0.0f;
    current = 0.0f;
    power = 0.0f;

    return ESP_OK;
}

esp_err_t battery_sensor_read(uint8_t *battery_level)
{
    if (battery_level == NULL)
    {
        ESP_LOGE(TAG, "Battery level pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    bus_voltage = 0.0f;

    // Leer el voltaje del bus
    esp_err_t ret = ina219_get_bus_voltage(&dev, &bus_voltage);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read bus voltage");
        return ret;
    }

     // Verificar que el voltaje esté dentro de un rango razonable
    if (bus_voltage < 6.0 || bus_voltage > 8.4)
    {
        ESP_LOGW(TAG, "Voltage out of expected range: %.2fV", bus_voltage);
        *battery_level = 0; // Nivel de batería inválido
        return ESP_OK;
    }

    // Calcular el porcentaje de carga basado en el rango [6.0V - 8.4V]
    float normalized_voltage = (bus_voltage - MIN_CHARGE) / (FULL_CHARGE - MIN_CHARGE);
    *battery_level = (uint8_t)(normalized_voltage * 100);

    // Asegurarse de que el porcentaje esté dentro de los límites [0-100]
    if (*battery_level > 100)
        *battery_level = 100;

    ESP_LOGI(TAG, "Battery level: %u%% (Voltage: %.2fV)", *battery_level, bus_voltage);

    return ESP_OK;
}