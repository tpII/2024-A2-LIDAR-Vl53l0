#include "mqtt_handler.h"
#include "mqtt_client.h"
#include "esp_log.h"

#define MESSAGE_MAX_LENGTH 40

//TOPICS DEFINITIONS
#define INSTRUCCION_MESSAGE "Instruction"
#define CONTROL_MESSAGE "Messages"
#define MAPPING_VALUE "Mapping"
#define BATTERY_VALUE "Battery"


static const char *TAG = "MQTT_HANDLER";

esp_err_t getInstruccionMessage(char *msg) {

    /*esp_err_t err = mqtt_get(INSTRUCCION_MESSAGE, msg, MESSAGE_MAX_LENGTH);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Error getting instruccion message: %s", esp_err_to_name(err));
        return err;
    }*/
    return ESP_OK;
}

esp_err_t sendControlMessage(const char *msg, const size_t length) {
    const char *key[1] = {"message"};
    const char *values[1] = {msg};
    /*esp_err_t err = mqtt_post(CONTROL_MESSAGE, key ,values, 1);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Error sending control message: %s", esp_err_to_name(err));
        return err;
    }*/
    return ESP_OK;
}

esp_err_t sendMappingValue(const uint16_t distance, const uint16_t angle) {
    const char *msg[2];
    char buffer1[6], buffer2[6];
    snprintf(buffer1, sizeof(buffer1), "%d", distance);
    snprintf(buffer2, sizeof(buffer2), "%d", angle);
    msg[0] = buffer1;
    msg[1] = buffer2;
    const char *keys[2]={"distance", "angle"};
    /*esp_err_t err = mqtt_post(MAPPING_VALUE, keys, msg, 2);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Error sending sensor value: %s", esp_err_to_name(err));
        return err;
    }*/
    return ESP_OK;
}