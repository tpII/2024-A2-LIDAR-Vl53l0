#include "backend_comm.h"

static const char *TAG = "Backend Communication";

esp_err_t backend_init() {

    esp_err_t err = mqtt_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client.");
        // ERROR AL REPORT LOG
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t send_message_to_backend(const char *message) {
    const char *topic = "/backend/esp32-msg";
    char payload[256];
    snprintf(payload, sizeof(payload), "%s", message);

    esp_err_t err = mqtt_publish(topic, message);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to publish a message.");
        // ERROR AL REPORT LOG
        return ESP_FAIL;
    }

    return ESP_OK;

}

esp_err_t backend_disconnect() {
    esp_err_t err = mqtt_disconnect();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to disconnect client.");
        // ERROR AL REPORT LOG
        return ESP_FAIL;
    }

    return ESP_OK;
}
