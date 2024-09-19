#include "http_handler.h"
#include "http_client.h"

#define MESSAGE_MAX_LENGTH 40
#define INSTRUCCION_MESSAGE "instruccion"
#define CONTROL_MESSAGE "message"
#define SENSOR_MESSAGE "sensorValue"

esp_err_t getInstruccionMessage(char *msg) {

    esp_err_t err = http_get(INSTRUCCION_MESSAGE, msg, MESSAGE_MAX_LENGTH);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Error getting instruccion message: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}

