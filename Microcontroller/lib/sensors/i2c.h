#ifndef _I2C_H_
#define _I2C_H_

#include "esp_err.h"

#define I2C_MASTER_SCL_IO 22               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0           /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 400000          /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0        /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0        /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000         /*!< I2C timeout in ms */



esp_err_t i2c_master_init(void);

esp_err_t i2c_master_write_slave(uint8_t, uint8_t *, size_t);

esp_err_t i2c_master_read_slave(uint8_t, uint8_t *, size_t);

#endif