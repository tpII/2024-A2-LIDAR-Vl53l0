
#ifndef _I2C_H_
#define _I2C_H_

#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO 22               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0           /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000          /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0        /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0        /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000         /*!< I2C timeout in ms */

#define IA219_ADDR 0x1A                    /*!< slave address for IA219 sensor */
#define VL53L0X_ADDR 0x29                  /*!< slave address for VL53L0X sensor */

//Function declarations

esp_err_t i2c_master_init(void);

#endif