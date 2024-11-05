#include "i2c.h"

static const char *TAG = "I2C";

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
        ESP_LOGE(TAG, "Error in i2c_param_config: %s", esp_err_to_name(err));
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

#define DEFAULT_SLAVE_ADDRESS (0x29)

typedef enum
{
    ADDR_SIZE_8BIT,
    ADDR_SIZE_16BIT
} addr_size_t;

typedef enum
{
    REG_SIZE_8BIT,
    REG_SIZE_16BIT,
    REG_SIZE_32BIT
} reg_size_t;

/* Read a register of size reg_size at address addr.
 * NOTE: The bytes are read from MSB to LSB. */
static bool read_reg(uint16_t addr, reg_size_t reg_size, uint8_t *data) {
    bool success = false;

    uint8_t register_addr = 0x1E; //CHEQUEAR SI ESTE CAMBIO SIRVIÓ O NO 

    // Crear un comando I2C
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL) {
        ESP_LOGE(TAG, "Error al crear comando I2C");
        return false;
    }

    // Iniciar la transferencia I2C
    i2c_master_start(cmd);

    // Escribir la dirección del dispositivo en el bus (modo escritura)
    if (i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true) != ESP_OK) {
        ESP_LOGE(TAG, "Error al escribir la dirección del dispositivo");
        i2c_cmd_link_delete(cmd);
        return false;
    }

    // Escribir el registro que queremos leer
    if (i2c_master_write_byte(cmd, register_addr, true) != ESP_OK) {
        ESP_LOGE(TAG, "Error al escribir el registro");
        i2c_cmd_link_delete(cmd);
        return false;
    }

    // Enviar el comando de repetición para leer (modo lectura)
    i2c_master_start(cmd);
    if (i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, true) != ESP_OK) {
        ESP_LOGE(TAG, "Error al escribir la dirección del dispositivo en modo lectura");
        i2c_cmd_link_delete(cmd);
        return false;
    }

    // Leer los datos
    switch (reg_size) {
        case REG_SIZE_8BIT:
            if (i2c_master_read(cmd, data, 1, I2C_MASTER_LAST_NACK) == ESP_OK) {
                success = true;
            }
            break;
        case REG_SIZE_16BIT:
            if (i2c_master_read(cmd, data + 1, 1, I2C_MASTER_NACK) == ESP_OK) {
                if (i2c_master_read(cmd, data, 1, I2C_MASTER_LAST_NACK) == ESP_OK) {
                    success = true;
                }
            }
            break;
        case REG_SIZE_32BIT:
            ESP_LOGE(TAG, "Tamaño de registro de 32 bits no soportado");
            break;
    }

    // Terminar la transferencia I2C
    i2c_master_stop(cmd);

    // Enviar el comando I2C y verificar el éxito
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    if (ret == ESP_OK) {
        // La lectura fue exitosa
        success = true;
    } else {
        ESP_LOGE(TAG, "Error en la transferencia I2C en read_reg: %s", esp_err_to_name(ret));
        success = false; // Asegúrate de que success sea falso si hay un error
    }

    // Liberar el comando I2C
    i2c_cmd_link_delete(cmd);
    return success;
}


static bool read_reg_bytes(addr_size_t addr_size, uint16_t addr, uint8_t *bytes, uint16_t byte_count) {
    bool success = false;

    // Crear un comando I2C
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL) {
        ESP_LOGE(TAG, "Error al crear comando I2C");
        return false;
    }

    // Iniciar la transferencia I2C en modo escritura para enviar la dirección del registro
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);

    // Enviar la dirección del registro (1 o 2 bytes según addr_size)
    if (addr_size == ADDR_SIZE_16BIT) {
        i2c_master_write_byte(cmd, (addr >> 8) & 0xFF, true); // Byte alto de la dirección
    }
    i2c_master_write_byte(cmd, addr & 0xFF, true); // Byte bajo de la dirección

    // Repetir la condición de inicio para cambiar a modo lectura
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, true);

    // Leer los bytes en el buffer
    for (int i = 0; i < byte_count; i++) {
        if (i == byte_count - 1) {
            // Último byte debe terminar con NACK
            i2c_master_read_byte(cmd, &bytes[i], I2C_MASTER_NACK);
        } else {
            // Leer el byte con ACK
            i2c_master_read_byte(cmd, &bytes[i], I2C_MASTER_ACK);
        }
    }

    // Finalizar la transferencia I2C
    i2c_master_stop(cmd);

    // Ejecutar el comando y verificar si fue exitoso
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    if (ret == ESP_OK) {
        success = true;
    } else {
        ESP_LOGE(TAG, "Error en la transferencia I2C en read_reg_bytes: %s", esp_err_to_name(ret));
    }

    // Liberar el comando I2C
    i2c_cmd_link_delete(cmd);

    return success;
}

bool i2c_read_addr8_data8(uint8_t addr, uint8_t *data)
{
    return read_reg(addr, REG_SIZE_8BIT, data);
}

bool i2c_read_addr8_data16(uint8_t addr, uint16_t *data)
{
    return read_reg(addr, REG_SIZE_16BIT, (uint8_t *)data);
}

bool i2c_read_addr16_data8(uint16_t addr, uint8_t *data)
{
    return read_reg(addr, REG_SIZE_8BIT, data);
}

bool i2c_read_addr16_data16(uint16_t addr, uint16_t *data)
{
    return read_reg(addr, REG_SIZE_16BIT, (uint8_t *)data);
}

bool i2c_read_addr8_data32(uint16_t addr, uint32_t *data)
{
    return read_reg(addr, REG_SIZE_32BIT, (uint8_t *)data);
}

bool i2c_read_addr16_data32(uint16_t addr, uint32_t *data)
{
    return read_reg(addr, REG_SIZE_32BIT, (uint8_t *)data);
}

bool i2c_read_addr8_bytes(uint8_t start_addr, uint8_t *bytes, uint16_t byte_count)
{
    return read_reg_bytes(ADDR_SIZE_8BIT, start_addr, bytes, byte_count);
}

/* Write data to a register of size reg_size at address addr.
 * NOTE: Writes the most significant byte (MSB) first. */
static bool write_reg(addr_size_t addr_size, uint16_t addr, reg_size_t reg_size, uint16_t data) {
    bool success = false;

    // Crear un comando I2C
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL) {
        ESP_LOGE(TAG, "Error al crear comando I2C");
        return false;
    }

    // Iniciar la transferencia I2C
    i2c_master_start(cmd);

    // Escribir la dirección del dispositivo en el bus (modo escritura)
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);

    // Escribir el registro en función del tamaño
    switch (reg_size) {
    case REG_SIZE_8BIT:
        i2c_master_write_byte(cmd, data & 0xFF, true); // Enviar el dato de 8 bits
        success = true;
        break;
    case REG_SIZE_16BIT:
        // Enviar primero el byte más significativo
        i2c_master_write_byte(cmd, (data >> 8) & 0xFF, true); // Byte alto
        // Enviar el byte menos significativo
        i2c_master_write_byte(cmd, data & 0xFF, true);         // Byte bajo
        success = true;
        break;
    case REG_SIZE_32BIT:
        ESP_LOGE(TAG, "Tamaño de registro de 32 bits no soportado");
        success = false;
        break;
    }

    // Terminar la transferencia I2C
    i2c_master_stop(cmd);

    // Enviar el comando I2C y verificar el éxito
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    if (ret == ESP_OK) {
        success = true;
    } else {
        ESP_LOGE(TAG, "Error en la transferencia I2C en write_reg: %s", esp_err_to_name(ret));
    }

    // Liberar el comando I2C
    i2c_cmd_link_delete(cmd);

    return success;
}

static bool write_reg_bytes(uint16_t addr, uint8_t *bytes, uint16_t byte_count) {
    bool success = false;

    // Crear un comando I2C para la transferencia
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL) {
        ESP_LOGE(TAG, "Error al crear comando I2C");
        return false;
    }

    // Iniciar la transferencia I2C
    i2c_master_start(cmd);

    // Escribir la dirección del dispositivo en el bus (modo escritura)
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);

    // Escribir los bytes al registro del dispositivo
    for (uint16_t i = 0; i < byte_count; i++) {
        i2c_master_write_byte(cmd, bytes[i], true); // Enviar cada byte con verificación de ACK
    }

    // Terminar la transferencia I2C
    i2c_master_stop(cmd);

    // Enviar el comando I2C y verificar el éxito
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
    if (ret == ESP_OK) {
        success = true;
    } else {
        ESP_LOGE(TAG, "Error en la transferencia I2C en write_reg_bytes: %s", esp_err_to_name(ret));
    }

    // Liberar el comando I2C
    i2c_cmd_link_delete(cmd);

    return success;
}

bool i2c_write_addr8_data8(uint8_t addr, uint8_t value)
{
    return write_reg(ADDR_SIZE_8BIT, addr, REG_SIZE_8BIT, value);
}

bool i2c_write_addr8_data16(uint8_t addr, uint16_t value)
{
    return write_reg(ADDR_SIZE_8BIT, addr, REG_SIZE_16BIT, value);
}

bool i2c_write_addr16_data8(uint16_t addr, uint8_t value)
{
    return write_reg(ADDR_SIZE_16BIT, addr, REG_SIZE_8BIT, value);
}

bool i2c_write_addr16_data16(uint16_t addr, uint16_t value)
{
    return write_reg(ADDR_SIZE_16BIT, addr, REG_SIZE_16BIT, value);
}
bool i2c_write_addr8_bytes(uint8_t start_addr, uint8_t *bytes, uint16_t byte_count)
{
    return write_reg_bytes(start_addr, bytes, byte_count);
}

// void i2c_set_slave_address(uint8_t addr)
// {
//     UCB0I2CSA = addr;
// }

/*
static esp_err_t i2c_set_slave_address(i2c_port_t i2c_num, uint8_t addr)
{
    // Cambia la dirección del esclavo utilizando la configuración del maestro
    return i2c_set_slave_addr(i2c_num, addr);
}
*/