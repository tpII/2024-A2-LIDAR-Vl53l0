#include "backend_comm.h"

static const char *TAG = "Backend Communication";
static const char * get_topic(const char *ESP_TAG);

esp_err_t backend_init() {

    esp_err_t err = mqtt_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client.");
        // ERROR AL REPORT LOG
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t send_message_to_backkend(const char* ESP_TAG, const char *msg) {
    const char *topic = "/backend/esp32-msg";
    /* segun el tag enviar a diferents topics
    const char *topic = get_topic(ESP_TAG); 
    if (topic == NULL) {
        ESP_LOGE(TAG, "ESP_TAG received unrecognized.");
        ENVIAR AL REPORT LOG
        return ESP_FAIL;
    }
    */
    const char *values[2];
    const char *keys[2] = {"tag", "message"};
    char esp_tag[256];
    char esp_msg[256];
    snprintf(esp_tag, sizeof(esp_tag), "%s", ESP_TAG);
    snprintf(esp_msg, sizeof(esp_msg), "%s", msg);
    values[0] = esp_tag;
    values[1] = esp_msg;
    cJSON *json = NULL;

    esp_err_t err = create_json_data(&json, keys, values, 2);
    print_json_data(json);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error creating json data: %s", esp_err_to_name(err));
        // ERROR A REPORT LOG
        return err;
    }

    esp_err_t err2 = mqtt_publish(topic, json);
    if (err2 != ESP_OK) {
        ESP_LOGE(TAG, "Failed to publish a message.",  esp_err_to_name(err));
        // ERROR AL REPORT LOG
        return ESP_FAIL;
    }

    return ESP_OK;
}

static const char * get_topic(const char *ESP_TAG) {
    if (ESP_TAG == "ERROR") {
       return "/backend/esp32-msg/error";
    }
    if (ESP_TAG == "WARNING") {
       return "/backend/esp32-msg/warning";
    }
    if (ESP_TAG == "SUCCESS") {
       return "/backend/esp32-msg/success";
    }
    if (ESP_TAG == "INFO") {
       return "/backend/esp32-msg/info";
    }
    
    return NULL;
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
