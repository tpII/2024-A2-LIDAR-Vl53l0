#include "i2c.h"



esp_err_t i2c_master_init(void){
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    esp_error_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if(err != ESP_OK){
        ESP_LOGE("I2C", "Error in i2c_param_config: %s", esp_err_to_name(err));
        //Seng Msg to User and Turn ON error led
        return err;
    }
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

esp_err_t i2c_master_write_slave(uint8_T slave_addr, uint8_t *data_wr, size_t size){
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
    i2c_master_read(cmd, data_rd, size, I2C_MASTER_LAST_ACK); // Leer los datos
    i2c_stop(cmd);   // Finalizar la transmisió
    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    i2c_cmd_link_delete(cmd);   // Liberar el handle del comando
    return err;
}

