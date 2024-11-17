#include "i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static SemaphoreHandle_t bus_semaphore;

static const char *TAG = "I2C";


esp_err_t i2c_init()
{

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
    return err;
}


bool i2c_get_bus(){
    if(xSemaphoreTake(bus_semaphore,portMAX_DELAY) == pdTRUE){
        return true;
    }
    return false;
}

bool i2c_give_bus(){
    return xSemaphoreGive(bus_semaphore);
}