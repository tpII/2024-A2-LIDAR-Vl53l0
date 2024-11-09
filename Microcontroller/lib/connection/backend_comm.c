#include "backend_comm.h"

static const char *TAG = "Backend Communication";

esp_err_t send_message_to_backend(const char* ESP_TAG, const char *msg, const char *msg_type) {

    esp_err_t err = mqtt_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client.");
        // ERROR AL REPORT LOG
        return ESP_FAIL;
    }

    err = sendControlMessage(ESP_TAG, msg, msg_type);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send control message.");
        // ERROR AL REPORT LOG
        return ESP_FAIL;
    }

    err = mqtt_disconnect();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to disconnect client.");
        // ERROR AL REPORT LOG
        return ESP_FAIL;
    }

    return ESP_OK;
}
