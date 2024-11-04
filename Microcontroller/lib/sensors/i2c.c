#include "i2c.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2c.h"

esp_err_t i2c_master_init(){
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if(err != ESP_OK){
        ESP_LOGE("I2C", "Error in i2c_param_config: %s", esp_err_to_name(err));
        //Seng Msg to User and Turn ON error led
        return err;
    }
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

esp_err_t i2c_master_write_slave(uint8_t slave_addr, uint8_t *data_wr, size_t size){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);  //Iniciar la transmision
    i2c_master_write_byte(cmd, (slave_addr << 1) | I2C_MASTER_WRITE, true); // Escribir dirección y operación (write)
    i2c_master_write(cmd, data_wr, size, true); // Escribir los datos
    i2c_master_stop(cmd);   // Finalizar la transmisió
    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);   // Liberar el handle del comando
    return err;
}

esp_err_t i2c_master_read_slave(uint8_t slave_addr, uint8_t *data_rd, size_t size){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);  //Iniciar la transmision
    i2c_master_write_byte(cmd, (slave_addr << 1) | I2C_MASTER_READ, true); // Escribir dirección y operación (read)
    i2c_master_read(cmd, data_rd, size, I2C_MASTER_LAST_NACK); // Leer los datos
    i2c_master_stop(cmd);   // Finalizar la transmisió
    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);   // Liberar el handle del comando
    return err;
}

//---------------------------------------WRAPPER FUNCTIONS FOR THE VL53LOX---------------------------------------

// Funciones de lectura

bool i2c_read_addr8_data8(uint8_t addr, uint8_t *data) {
    esp_err_t ret = i2c_master_write_slave(I2C_MASTER_NUM, &addr, 1);
    if (ret != ESP_OK) return false;
    ret = i2c_master_read_slave(I2C_MASTER_NUM, data, 1);
    return ret == ESP_OK;
}

bool i2c_read_addr8_data16(uint8_t addr, uint16_t *data) {
    esp_err_t ret = i2c_master_write_slave(I2C_MASTER_NUM, &addr, 1);
    if (ret != ESP_OK) return false;
    uint8_t buf[2];
    ret = i2c_master_read_slave(I2C_MASTER_NUM, buf, 2);
    *data = (buf[0] << 8) | buf[1];
    return ret == ESP_OK;
}

bool i2c_read_addr16_data8(uint16_t addr, uint8_t *data) {
    uint8_t buf[2] = { addr >> 8, addr & 0xFF };
    esp_err_t ret = i2c_master_write_slave(I2C_MASTER_NUM, buf, 2);
    if (ret != ESP_OK) return false;
    return i2c_master_read_slave(I2C_MASTER_NUM, data, 1) == ESP_OK;
}

bool i2c_read_addr16_data16(uint16_t addr, uint16_t *data) {
    uint8_t buf[2] = { addr >> 8, addr & 0xFF };
    esp_err_t ret = i2c_master_write_slave(I2C_MASTER_NUM, buf, 2);
    if (ret != ESP_OK) return false;
    uint8_t data_buf[2];
    ret = i2c_master_read_slave(I2C_MASTER_NUM, data_buf, 2);
    *data = (data_buf[0] << 8) | data_buf[1];
    return ret == ESP_OK;
}

bool i2c_read_addr8_data32(uint16_t addr, uint32_t *data) {
    esp_err_t ret = i2c_master_write_slave(I2C_MASTER_NUM, &addr, 1);
    if (ret != ESP_OK) return false;
    uint8_t buf[4];
    ret = i2c_master_read_slave(I2C_MASTER_NUM, buf, 4);
    *data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
    return ret == ESP_OK;
}

bool i2c_read_addr16_data32(uint16_t addr, uint32_t *data) {
    uint8_t buf[2] = { addr >> 8, addr & 0xFF };
    esp_err_t ret = i2c_master_write_slave(I2C_MASTER_NUM, buf, 2);
    if (ret != ESP_OK) return false;
    uint8_t data_buf[4];
    ret = i2c_master_read_slave(I2C_MASTER_NUM, data_buf, 4);
    *data = (data_buf[0] << 24) | (data_buf[1] << 16) | (data_buf[2] << 8) | data_buf[3];
    return ret == ESP_OK;
}

bool i2c_read_addr8_bytes(uint8_t start_addr, uint8_t *bytes, uint16_t byte_count) {
    esp_err_t ret = i2c_master_write_slave(I2C_MASTER_NUM, &start_addr, 1);
    if (ret != ESP_OK) return false;
    return i2c_master_read_slave(I2C_MASTER_NUM, bytes, byte_count) == ESP_OK;
}

// Funciones de escritura

bool i2c_write_addr8_data8(uint8_t addr, uint8_t value) {
    uint8_t buf[2] = { addr, value };
    return i2c_master_write_slave(I2C_MASTER_NUM, buf, 2) == ESP_OK;
}

bool i2c_write_addr8_data16(uint8_t addr, uint16_t value) {
    uint8_t buf[3] = { addr, (value >> 8) & 0xFF, value & 0xFF };
    return i2c_master_write_slave(I2C_MASTER_NUM, buf, 3) == ESP_OK;
}

bool i2c_write_addr16_data8(uint16_t addr, uint8_t value) {
    uint8_t buf[3] = { addr >> 8, addr & 0xFF, value };
    return i2c_master_write_slave(I2C_MASTER_NUM, buf, 3) == ESP_OK;
}

bool i2c_write_addr16_data16(uint16_t addr, uint16_t value) {
    uint8_t buf[4] = { addr >> 8, addr & 0xFF, (value >> 8) & 0xFF, value & 0xFF };
    return i2c_master_write_slave(I2C_MASTER_NUM, buf, 4) == ESP_OK;
}

bool i2c_write_addr8_bytes(uint8_t start_addr, uint8_t *bytes, uint16_t byte_count) {
    uint8_t buf[byte_count + 1];
    buf[0] = start_addr;
    memcpy(&buf[1], bytes, byte_count);
    return i2c_master_write_slave(I2C_MASTER_NUM, buf, byte_count + 1) == ESP_OK;
}