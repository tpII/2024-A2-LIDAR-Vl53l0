#ifndef _I2C_H_
#define _I2C_H_

#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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


/**
 * Wrapper functions for reading from registers with different address
 * and data sizes.
 * @return True if success, False if error
 * NOTE: Reads the most significant byte first if multi-byte data.
 * NOTE: Polling-based
 */
bool i2c_read_addr8_data8(uint8_t addr, uint8_t *data);
bool i2c_read_addr8_data16(uint8_t addr, uint16_t *data);
bool i2c_read_addr16_data8(uint16_t addr, uint8_t *data);
bool i2c_read_addr16_data16(uint16_t addr, uint16_t *data);
bool i2c_read_addr8_data32(uint16_t addr, uint32_t *data);
bool i2c_read_addr16_data32(uint16_t addr, uint32_t *data);

/**
 * Read byte_count bytes from address addr
 * NOTE: Polling-based
 */
bool i2c_read_addr8_bytes(uint8_t start_addr, uint8_t *bytes, uint16_t byte_count);

/**
 * Wrapper functions for writing to registers with different address
 * and data sizes.
 * @return True if success, False if error
 * NOTE: Writes the most significant byte if multi-byte data.
 * NOTE: Polling-based
 */
bool i2c_write_addr8_data8(uint8_t addr, uint8_t data);
bool i2c_write_addr8_data16(uint8_t addr, uint16_t data);
bool i2c_write_addr16_data8(uint16_t addr, uint8_t data);
bool i2c_write_addr16_data16(uint16_t addr, uint16_t data);

/**
 * Write an array of bytes of size byte_count to address addr.
 * NOTE: Polling-based
 */
bool i2c_write_addr8_bytes(uint8_t start_addr, uint8_t *bytes, uint16_t byte_count);

#endif