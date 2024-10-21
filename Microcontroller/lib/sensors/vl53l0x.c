#include "vl53l0x.h"
#include "driver/i2c.h"
#include "esp_log.h"

static const char* TAG = "VL53L0X";

// Escribir un valor a un registro del VL53L0X
esp_err_t VL53L0X_write_register(uint8_t reg, uint8_t value) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (VL53L0X_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, value, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error writing to register 0x%02X", reg);
    }
    
    return ret;
}

// Leer datos desde un registro del VL53L0X
esp_err_t VL53L0X_read_register(uint8_t reg, uint8_t *data, size_t len) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (VL53L0X_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (VL53L0X_I2C_ADDRESS << 1) | I2C_MASTER_READ, true);

    if (len > 1) {
        i2c_master_read(cmd, data, len - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data + len - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error reading from register 0x%02X", reg);
    }
    
    return ret;
}

// Inicializar I2C y el sensor VL53L0X
esp_err_t VL53L0X_init(void) {
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000
    };

    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error configuring I2C");
        return err;
    }

    err = i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error installing I2C driver");
        return err;
    }

    ESP_LOGI(TAG, "I2C initialized successfully");
    return ESP_OK;
}

// Iniciar el sensor VL53L0X para que comience a medir
esp_err_t VL53L0X_start_ranging(void) {
    VL53L0X_write_register(0x80, 0x01);
    VL53L0X_write_register(0xFF, 0x01);
    VL53L0X_write_register(0x00, 0x00);
    VL53L0X_write_register(0x91, 0x3c);
    VL53L0X_write_register(0x00, 0x01);
    VL53L0X_write_register(0xFF, 0x00);
    VL53L0X_write_register(0x80, 0x00);
    
    ESP_LOGI(TAG, "VL53L0X started ranging");
    return ESP_OK;
}

// Leer la distancia desde el sensor VL53L0X
uint16_t VL53L0X_read_range(void) {
    uint8_t range_data[2];
    VL53L0X_read_register(0x1E, range_data, 2);  // Leer 2 bytes desde el registro 0x1E

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
//     esp_err_t ret = VL53L0X_init();
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
