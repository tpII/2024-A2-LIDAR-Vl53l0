#include "http_handler.h"
#include "http_client.h"
#include "esp_log.h"
#include "esp_err.h"

#define MESSAGE_MAX_LENGTH 40
#define INSTRUCCION_MESSAGE "instruccion"
#define CONTROL_MESSAGE "message"
#define SENSOR_MESSAGE "sensor"

static const char *TAG = "HTTP_HANDLER";

esp_err_t getInstruccionMessage(char *msg) {

    esp_err_t err = http_get(INSTRUCCION_MESSAGE, msg, MESSAGE_MAX_LENGTH);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Error getting instruccion message: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}

esp_err_t sendControlMessage(const char *msg, const size_t length) {
    const char *key[1] = {"message"};
    const char *values[1] = {msg};
    esp_err_t err = http_post(CONTROL_MESSAGE, key ,values, 1);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Error sending control message: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}

esp_err_t sendSensorValue(const uint16_t distance, const uint16_t angle) {
    //char msg[2][6];
    const char *msg[2];
    char buffer1[6], buffer2[6];
    //snprintf(msg[0], sizeof(msg[0]), "%d", distance);
    //snprintf(msg[1], sizeof(msg[1]), "%d", angle);
    snprintf(buffer1, sizeof(buffer1), "%d", distance);
    snprintf(buffer2, sizeof(buffer2), "%d", angle);
    msg[0] = buffer1;
    msg[1] = buffer2;
    printf("values: %s - %s\n",buffer1,buffer2);
    const char *keys[2]={"distance", "angle"};
    printf("Keys: %s,%s\n",keys[0],keys[1]);
    esp_err_t err = http_post(SENSOR_MESSAGE, keys, msg, 2);
    printf("BACK FROM POST");
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Error sending sensor value: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}