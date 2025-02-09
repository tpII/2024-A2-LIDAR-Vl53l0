/**
 * @file battery.c
 * @brief Implementation of battery level measurement using the INA219 sensor.
 *
 * This module initializes and reads battery voltage using the INA219 current sensor.
 * The battery level is calculated based on voltage readings, with a range from 6.4V (0%)
 * to 8.4V (100%).
 *
 * @author [Your Name]
 * @date 2025-02-09
 */
#include "battery.h"
#include "esp_log.h"
#include "ina219v2.h"
#include "string.h"
#include "debug_helper.h"

#define CONFIG_SHUNT_RESISTOR_MILLI_OHM 100 ///< Shunt resistor value in mOhm, defined in sdkconfig
#define FULL_CHARGE 8.4                     ///< Full charge voltage (8.4V)
#define MIN_CHARGE 6.4                      ///< Minimum battery voltage (6.4V, equivalent to 0%)

static const char *TAG = "BATTERY"; ///< Log tag for debugging

static ina219_t dev;        ///< INA219 sensor instance
static float bus_voltage;   ///< Measured bus voltage (V)
static float shunt_voltage; ///< Measured shunt voltage (mV)
static float current;       ///< Measured current (mA)
static float power;         ///< Measured power (mW)

/**
 * @brief Initializes the INA219 battery sensor.
 *
 * This function sets up the INA219 sensor by configuring the I2C address and performing
 * calibration. It also verifies that the shunt resistor value is valid.
 *
 * @return
 *  - ESP_OK if initialization was successful.
 *  - ESP_ERR_INVALID_ARG if the configured shunt resistor value is invalid.
 *  - Another error code if sensor initialization fails.
 */
esp_err_t battery_sensor_init()
{
    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Setting memory to 0"));

    memset(&dev, 0, sizeof(ina219_t));

    if (CONFIG_SHUNT_RESISTOR_MILLI_OHM <= 0)
    {
        DEBUGING_ESP_LOG(ESP_LOGE(TAG, "Invalid shunt resistor value: %d mOhm", CONFIG_SHUNT_RESISTOR_MILLI_OHM));
        LOG_MESSAGE_E(TAG, "Invalid shunt resistor value");
        return ESP_ERR_INVALID_ARG;
    }

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Initalizing INA219"));
    dev.i2c_addr = INA219_ADDRESS;
    esp_err_t err = ina219_init(&dev);
    if (err != ESP_OK)
    {
        DEBUGING_ESP_LOG(ESP_LOGE(TAG, "INA219 initialization failed: %s", esp_err_to_name(err)));
        LOG_MESSAGE_E(TAG, "INA219 initialization failed");
        return err;
    }

    // ESP_LOGI(TAG, "Configuring INA219");
    // ESP_ERROR_CHECK(ina219_configure(&dev, INA219_BUS_RANGE_16V, INA219_GAIN_0_125,
    //                                  INA219_RES_12BIT_1S, INA219_RES_12BIT_1S,
    //                                  INA219_MODE_CONT_SHUNT_BUS));

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Calibrating INA219"));

    DEBUGING_ESP_LOG(ESP_ERROR_CHECK(ina219_calibrate(&dev, (float)CONFIG_SHUNT_RESISTOR_MILLI_OHM / 1000.0f)));

    bus_voltage = 0.0f;
    shunt_voltage = 0.0f;
    current = 0.0f;
    power = 0.0f;

    return ESP_OK;
}

/**
 * @brief Reads the battery level as a percentage.
 *
 * This function reads the bus voltage from the INA219 sensor and calculates
 * the battery level as a percentage based on a range of [6.4V - 8.4V].
 *
 * @param[out] battery_level Pointer to a variable where the battery level (0-100%) will be stored.
 *
 * @return
 *  - ESP_OK if reading was successful.
 *  - ESP_ERR_INVALID_ARG if the pointer is NULL.
 *  - Another error code if sensor reading fails.
 */
esp_err_t battery_sensor_read(uint8_t *battery_level)
{
    if (battery_level == NULL)
    {
        DEBUGING_ESP_LOG(ESP_LOGE(TAG, "Battery level pointer is NULL"));
        LOG_MESSAGE_E(TAG, "Battery level pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    bus_voltage = 0.0f;

    // Read shunt voltage
    esp_err_t ret = ina219_get_bus_voltage(&dev, &bus_voltage);
    if (ret != ESP_OK)
    {
        DEBUGING_ESP_LOG(ESP_LOGE(TAG, "Failed to read bus voltage"));
        LOG_MESSAGE_E(TAG, "Failed to read bus voltage");
        return ret;
    }

    // Check if voltage is within a valid range
    if (bus_voltage < 6.0 || bus_voltage > 8.4)
    {
        DEBUGING_ESP_LOG(ESP_LOGW(TAG, "Voltage out of expected range: %.2fV", bus_voltage));
        LOG_MESSAGE_W(TAG, "Voltage out of expected range");
        *battery_level = 0; // Nivel de batería inválido
        return ESP_OK;
    }

    // Calculate charge percentage based on the range [6.4V - 8.4V]
    float normalized_voltage = (bus_voltage - MIN_CHARGE) / (FULL_CHARGE - MIN_CHARGE);
    *battery_level = (uint8_t)(normalized_voltage * 100);

    // Ensure percentage is within [0-100] limits
    if (*battery_level > 100)
        *battery_level = 100;

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Battery level: %u%% (Voltage: %.2fV)", *battery_level, bus_voltage));

    return ESP_OK;
}