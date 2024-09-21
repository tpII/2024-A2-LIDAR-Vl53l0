#include "http_handler.h"
#include "http_client.h"


#define MESSAGE_MAX_LENGTH 40
#define INSTRUCCION_MESSAGE "instruccion"
#define CONTROL_MESSAGE "message"
#define SENSOR_MESSAGE "sensorValue"

esp_err_t getInstruccionMessage(char *msg) {

    esp_err_t err = http_get(INSTRUCCION_MESSAGE, *msg, MESSAGE_MAX_LENGTH);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Error getting instruccion message: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}

esp_err_t sendControlMessage(const char *msg, const size_t length) {
    char key = "message"
    esp_err_t err = http_post(CONTROL_MESSAGE, key ,&msg, 1);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Error sending control message: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}

esp_err_t sendSensorValue(const unit8_t distance, const unit8_t angle) {
    char msg[]={distance, angle};
    char keys[]={"distance", "angle"};
    esp_err_t err = http_post(SENSOR_MESSAGE, keys, msg, 2);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Error sending sensor value: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}