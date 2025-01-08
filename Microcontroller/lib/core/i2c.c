/**
 * @file i2c.c
 * @brief I2C Master Interface Library for ESP32
 * 
 * This library provides initialization and bus management for the I2C master 
 * interface on ESP32 devices, including semaphore-based access control for 
 * safe usage in multi-tasking environments.
 * 
 * The library handles the following:
 * - I2C master initialization with predefined configurations.
 * - Semaphore management for ensuring exclusive bus access.
 * - Functions to acquire and release the bus for multi-task applications.
 * 
 * @version 1.0
 * @date 2024-12-05
 * 
 * @author Guerrico Leonel (lguerrico@outlook.com)
 * 
 * @copyright Copyright (c) 2024 by Guerrico Leonel. All rights reserved.
 * 
 * @note 
 * - This library is designed for ESP32 using the ESP-IDF framework.
 * - Ensure pull-up resistors are connected to the I2C lines as required for the 
 *   I2C protocol to work correctly.
 * 
 */
#include "i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <stdint.h>

/**
 * @brief Static semaphore handle for managing I2C bus access.
 * 
 */
static SemaphoreHandle_t bus_semaphore;

/**
 * @brief Tag used for ESP-IDF logging.
 */
static const char *TAG = "I2C";

/**
 * @brief Initializes the I2C master interface.
 * 
 * This function configures the I2C hardware with predefined parameters such as 
 * GPIO pins, clock speed, and pull-up configuration. It creates a FreeRTOS 
 * binary semaphore to manage access to the I2C bus and ensures that the 
 * initialization is only performed once.
 * 
 * @note If initialization has already been performed, the function skips 
 * reinitialization and logs a message.
 * 
 * @return 
 * - `ESP_OK`: Initialization was successful or was already performed.
 * - `ESP_FAIL`: Semaphore creation failed.
 * - Other `esp_err_t`: Errors from `i2c_param_config` or `i2c_driver_install`.
 */
esp_err_t i2c_init()
{

    static bool is_initialized = false; // Estado de inicialización

    if(is_initialized){
        ESP_LOGI(TAG, "I2C already initialized, skipping initialization.");
        return ESP_OK;
    }

    bus_semaphore = xSemaphoreCreateBinary();

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .clk_flags = 0,
    };

    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error in i2c_param_config: %s", esp_err_to_name(err));
        // Seng Msg to User and Turn ON error led

        return err;
    }

    err = i2c_driver_install(I2C_MASTER_NUM,
                             conf.mode,
                             I2C_MASTER_RX_BUF_DISABLE,
                             I2C_MASTER_TX_BUF_DISABLE,
                             0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error in i2c_driver_install: %s", esp_err_to_name(err));
        // Seng Msg to User and Turn ON error led
        return err;
    }

    if (xSemaphoreGive(bus_semaphore) != pdTRUE)
    {
        ESP_LOGE(TAG, "Error Initializing I2C Bus Semaphore");
        return ESP_FAIL;
    }
    
    is_initialized = true;
    return err;
}


/**
 * @brief Acquires the I2C bus for exclusive access.
 * 
 * This function blocks until the semaphore controlling the I2C bus is available, 
 * ensuring that only one task can use the bus at a time.
 * 
 * @return 
 * - `true`: The bus was successfully acquired.
 * - `false`: Failed to acquire the bus.
 */
bool i2c_get_bus(){
    if(xSemaphoreTake(bus_semaphore,portMAX_DELAY) == pdTRUE){
        return true;
    }
    return false;
}

/**
 * @brief Releases the I2C bus after use.
 * 
 * This function releases the semaphore controlling the I2C bus, allowing other 
 * tasks to acquire it.
 * 
 * @return 
 * - `true`: The bus was successfully released.
 * - `false`: Failed to release the bus.
 */
bool i2c_give_bus(){
    return xSemaphoreGive(bus_semaphore);
}