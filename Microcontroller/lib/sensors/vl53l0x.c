#include "vl53l0x.h"
#include "driver/i2c.h"
#include "esp_log.h"

static const char* TAG = "VL53L0X";

// Iniciar el sensor VL53L0X para que comience a medir
esp_err_t VL53L0X_start_ranging(void) {
    i2c_master_write_slave(VL53L0X_I2C_ADDRESS, 0x80, 0x01);
    i2c_master_write_slave(VL53L0X_I2C_ADDRESS, 0x80, 0x01);
    i2c_master_write_slave(VL53L0X_I2C_ADDRESS, 0xFF, 0x01);
    i2c_master_write_slave(VL53L0X_I2C_ADDRESS, 0x00, 0x00);
    i2c_master_write_slave(VL53L0X_I2C_ADDRESS, 0x91, 0x3c);
    i2c_master_write_slave(VL53L0X_I2C_ADDRESS, 0x00, 0x01);
    i2c_master_write_slave(VL53L0X_I2C_ADDRESS, 0xFF, 0x00);
    i2c_master_write_slave(VL53L0X_I2C_ADDRESS, 0x80, 0x00);
    
    ESP_LOGI(TAG, "VL53L0X started ranging");
    return ESP_OK;
}

// Leer la distancia desde el sensor VL53L0X
uint16_t VL53L0X_ADDR_read_range(void) {
    uint8_t range_data[2];
    i2c_master_read_slave(VL53L0X_I2C_HIGH_BYTE, range_data, 2);  // Leer 2 bytes desde el registro VL53L0X_I2C_HIGH_BYTE

    uint16_t range = (range_data[0] << 8) | range_data[1];  // Combinar los bytes
    ESP_LOGI(TAG, "Range: %d mm", range);
    return range;
}

// Detener la medición del sensor VL53L0X
esp_err_t VL53L0X_stop_ranging(void) {
    ESP_LOGI(TAG, "VL53L0X stopped ranging");
    return ESP_OK;
}

// EJEMPLO DE main.c
// Aplicación principal
// #include "vl53l0x.h"

// void app_main(void) {
//     esp_err_t ret = i2c_master_init();
//     if (ret == ESP_OK) {
//         VL53L0X_start_ranging();
//         vTaskDelay(pdMS_TO_TICKS(100));  // Esperar un poco para que el sensor realice la medición
//         uint16_t distance = VL53L0X_read_range();
//         ESP_LOGI("APP", "Distance: %d mm", distance);
//         VL53L0X_stop_ranging();
//     } else {
//         ESP_LOGE("APP", "Failed to initialize VL53L0X");
//     }
// }
