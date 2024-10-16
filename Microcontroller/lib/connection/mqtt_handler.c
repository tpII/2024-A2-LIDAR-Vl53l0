#include "mqtt_handler.h"
#include "mqtt_server.h"
#include "json_helper.h"
#include "esp_log.h"

#define INSTRUCTIONS_BUFFER_SIZE 10
#define MESSAGE_MAX_LENGTH 40

// TOPICS DEFINITIONS
#define INSTRUCTION_MESSAGE "Instruction"
#define CONTROL_MESSAGE "Messages"
#define MAPPING_VALUE "Mapping"
#define BATTERY_VALUE "Battery"

const char *instructionbuffer[INSTRUCTIONS_BUFFER_SIZE];

static const char *TAG = "MQTT_HANDLER";

esp_err_t getInstruccionMessage(char *msg)
{

    return getInstruccionMessage(msg);
}

esp_err_t sendControlMessage(const char *msg, const size_t length)
{
    const char *key[1] = {"message"};
    const char *values[1] = {msg};
    cJSON *msg = NULL;

    esp_err_t err = create_json_data(&msg, key, values, 1);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error creating json data: %s", esp_err_to_name(err));
        return err;
    }

    esp_err_t err2 = mqtt_publish(CONTROL_MESSAGE, msg);
    if (err2 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error publishing Control Message: %s", esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}

esp_err_t sendMappingValue(const uint16_t distance, const uint16_t angle)
{
    const char *values[2];
    char buffer1[6], buffer2[6];
    snprintf(buffer1, sizeof(buffer1), "%d", distance);
    snprintf(buffer2, sizeof(buffer2), "%d", angle);
    values[0] = buffer1;
    values[1] = buffer2;
    const char *keys[2] = {"distance", "angle"};
    cJSON *msg = NULL;

    esp_err_t err = create_json_data(&msg, keys, values, 2);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error creating json data: %s", esp_err_to_name(err));
        return err;
    }

    esp_err_t err2 = mqtt_publish(MAPPING_VALUE, msg);
    if (err2 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error publishing Control Message: %s", esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}