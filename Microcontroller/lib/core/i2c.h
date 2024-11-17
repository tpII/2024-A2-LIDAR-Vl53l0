#ifndef _I2C_H_
#define _I2C_H_

#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define I2C_MASTER_SCL_IO GPIO_NUM_22               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO GPIO_NUM_21               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0           /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 400000          /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0        /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0        /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000         /*!< I2C timeout in ms */



esp_err_t i2c_init(void);

bool i2c_get_bus();

bool i2c_give_bus();

#endif