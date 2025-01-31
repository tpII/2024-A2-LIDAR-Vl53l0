/**
 * @file mqtt_handler.c
 * @author
 *      Guerrico Leonel (lguerrico@outlook.com)
 * @brief Implementation of MQTT message handling for the system
 *
 * This file provides the core functionality for interacting with an MQTT broker.
 * It includes functions to send control messages (informational, warning, and error),
 * mapping data (distance and angle), and any other MQTT-related operations.
 *
 * The functions implemented here are designed for:
 * - Sending JSON-encoded messages via MQTT.
 * - Handling control messages and mapping data.
 * - Interfacing with other modules through structured JSON payloads.
 *
 * @version 1.0
 * @date 2024-12-04
 *
 * @copyright
 * Copyright (c) 2024 by Guerrico Leonel. All rights reserved.
 */

#include "mqtt_handler.h"
#include "mqtt_server.h"
#include "json_helper.h"
#include "esp_log.h"
#include "instruction_buffer.h"

// Definitions
#define INSTRUCTIONS_BUFFER_SIZE 10       // Maximum number of instructions to store in buffer
#define INSTRUCTION_MESSAGE "Instruction" // Instruction Topic
#define CONTROL_MESSAGE "Messages"        // Message Topic
#define MAPPING_VALUE "Mapping"           // Mapping Value Topic
#define BATTERY_VALUE "Battery"           // Battery Level Topic
#define BARRIER "Barrier"

// Const
static const char *TAG = "MQTT_HANDLER"; // Library Tag

// Function Prototypes
static esp_err_t sendControlMessage(const char *, const char *, const char *);

/**
 * @brief Get the Instruction Message
 *
 * This function get instruction from instruction buffer in instruction_buffer library.
 *
 * @param[out] msg Instruction
 * @return
 *      - ESP_OK on success
 *      - ESP_FAIL on fail
 */
esp_err_t getInstruccionMessage(char *msg)
{

    return getInstruction(msg);
}

/**
 * @brief Sends a control message as a JSON payload.
 *
 * This funcion generate a JSON object from the parameters provided and send
 * it as a control message using the MQTT protocol.a64l
 *
 * The JSON structur is as follows:
 * {
 *  "tag": "<ESP_TAG>",
 *  "type": "<msg_type>",
 *  "message": "<msg>"
 *
 * }
 *
 * @param[in] ESP_TAG A label identifying the source library or component
 *                generating the message (e.g., "MQTT_HANDLER").
 * @param[in] msg_type Label that identify the severity of MessagePossible values are:
 *      - "Info": Informational messages.
 *      - "Warning": Warning messages.
 *      - "Error": Error messages.
 * @param[in] msg Message to send
 * @return
 *      - ESP_OK: If the message was successfully created and sent.
 *      - ESP_FAIL: If there was an error creating the JSON or sending the message.
 *
 * @note This function relies on the following functions:
 *      - `create_json_data`: Creates the JSON structure.
 *      - `print_json_data`: Logs the JSON for debugging.
 *      - `mqtt_publish`: Publishes the message via MQTT.
 *
 * @warning Ensure that `ESP_TAG`, `msg_type`, and `msg` are valid non-null
 *          strings to avoid undefined behavior.
 *
 */
static esp_err_t sendControlMessage(const char *ESP_TAG, const char *msg_type, const char *msg)
{
    const char *key[3] = {"tag", "type", "message"};
    const char *values[3] = {ESP_TAG, msg_type, msg};
    cJSON *json = NULL;

    esp_err_t err = create_json_data(&json, key, values, 3);
    print_json_data(json);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error creating json data: %s", esp_err_to_name(err));
        return err;
    }

    err = mqtt_publish(CONTROL_MESSAGE, json);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error publishing Control Message: %s", esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}

/**
 * @brief Sends an informational control message.
 *
 * This function sends a control message with the severity level set to "INFO".
 * It wraps the `sendControlMessage` function for convenience.
 *
 * @param[in] TAG A label identifying the source library or component generating
 *            the message (e.g., "MQTT_HANDLER").
 * @param[in] msg The informational message content to send.
 *
 * @return
 *      - ESP_OK: If the message was successfully sent.
 *      - ESP_FAIL: If there was an error sending the message.
 *
 * @see sendControlMessage
 */
esp_err_t sendInfoMesage(const char *TAG, const char *msg)
{
    return sendControlMessage(TAG, "INFO", msg);
}

/**
 * @brief Sends a warning control message.
 *
 * This function sends a control message with the severity level set to "WARNING".
 * It wraps the `sendControlMessage` function for convenience.
 *
 * @param[in] TAG A label identifying the source library or component generating
 *            the message (e.g., "MQTT_HANDLER").
 * @param[in] msg The warning message content to send.
 *
 * @return
 *      - ESP_OK: If the message was successfully sent.
 *      - ESP_FAIL: If there was an error sending the message.
 *
 * @see sendControlMessage
 */

esp_err_t sendWarningMesage(const char *TAG, const char *msg)
{
    return sendControlMessage(TAG, "WARNING", msg);
}

/**
 * @brief Sends an error control message.
 *
 * This function sends a control message with the severity level set to "ERROR".
 * It wraps the `sendControlMessage` function for convenience.
 *
 * @param[in] TAG A label identifying the source library or component generating
 *            the message (e.g., "MQTT_HANDLER").
 * @param[in] msg The error message content to send.
 *
 * @return
 *      - ESP_OK: If the message was successfully sent.
 *      - ESP_FAIL: If there was an error sending the message.
 *
 * @see sendControlMessage
 */

esp_err_t sendErrorMesage(const char *TAG, const char *msg)
{
    return sendControlMessage(TAG, "ERROR", msg);
}

/**
 * @brief Sends mapping data as a JSON payload.
 *
 * This function generates a JSON object containing the mapping data, specifically
 * the distance and angle, and sends it using the MQTT protocol.
 *
 * The JSON structure is as follows:
 * {
 *   "distance": "<distance>",
 *   "angle": "<angle>"
 * }
 *
 * @param[in] distance The distance value to include in the mapping data.
 * @param[in] angle The angle value to include in the mapping data.
 *
 * @return
 *      - ESP_OK: If the mapping data was successfully sent.
 *      - ESP_FAIL: If there was an error creating or sending the JSON payload.
 *
 * @note This function relies on the following:
 *      - `create_json_data`: Creates the JSON structure.
 *      - `print_json_data`: Logs the JSON for debugging.
 *      - `mqtt_publish`: Publishes the JSON payload via MQTT.
 *
 * @warning Ensure the `distance` and `angle` parameters are valid values.
 *          The function converts them to strings with a maximum buffer size
 *          of 6 characters.
 */

esp_err_t sendMappingValue(const uint16_t distance, const uint16_t angle)
{
    const char *values[2];
    char buffer1[6], buffer2[6];
    snprintf(buffer1, sizeof(buffer1), "%d", distance);
    snprintf(buffer2, sizeof(buffer2), "%d", angle);
    values[0] = buffer1;
    values[1] = buffer2;
    const char *keys[2] = {"distance", "angle"};
    cJSON *json = NULL;

    esp_err_t err = create_json_data(&json, keys, values, 2);

    print_json_data(json);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error creating json data: %s", esp_err_to_name(err));
        return err;
    }

    esp_err_t err2 = mqtt_publish(MAPPING_VALUE, json);
    if (err2 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error publishing Control Message: %s", esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}
esp_err_t sendBatteryLevel(uint8_t batteryLevel)
{
    const char *values[1];                                // Tamaño correcto para un solo valor
    char buffer[6];                                       // Espacio suficiente para representar valores de 0 a 255
    snprintf(buffer, sizeof(buffer), "%d", batteryLevel); // Usar snprintf para evitar desbordamientos
    values[0] = buffer;                                   // Asignar al índice válido (0)

    const char *keys[1] = {"level"};
    cJSON *json = NULL;

    // Crear el JSON
    esp_err_t err = create_json_data(&json, keys, values, 1);
    print_json_data(json); // Revisar si esta función necesita liberación de memoria o copias seguras

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error creating json data: %s", esp_err_to_name(err));
        return err;
    }

    // Publicar el mensaje
    err = mqtt_publish(BATTERY_VALUE, json);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error publishing control message: %s", esp_err_to_name(err));
       // cJSON_Delete(json); // Liberar memoria antes de salir
        return err;
    }

   // cJSON_Delete(json); // Liberar memoria al finalizar
   ESP_LOGI(TAG, "Battery level published");

    return ESP_OK;
}

esp_err_t sendBarrier()
{
    esp_err_t err;
    err = mqtt_publish(BATTERY_VALUE, "BARRIER");
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error publishing control message: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}