/**
 * @file mqtt_server.h
 * @author 
 *      Guerrico Leonel (lguerrico@outlook.com)
 * @brief MQTT Client Interface for ESP-IDF
 * 
 * This header defines the core functionality for interacting with an MQTT broker. 
 * It provides functions for initializing the MQTT client, publishing messages, 
 * and disconnecting from the broker.
 * 
 * @version 1.0
 * @date 2024-12-04
 * 
 * @copyright
 * Copyright (c) 2024 by Guerrico Leonel. All rights reserved.
 */

#ifndef _MQTT_SERVER_H_
#define _MQTT_SERVER_H_

#include "esp_err.h"

/**
 * @brief Initialize the MQTT client and connect to the broker
 * 
 * This function sets up the MQTT client, configures its connection parameters, 
 * and establishes a connection to the MQTT broker. It must be called before any 
 * other MQTT operations.
 * 
 * @return 
 *      - ESP_OK on successful connection
 *      - ESP_FAIL on failure to initialize or connect
 */
esp_err_t mqtt_start();

/**
 * @brief Publish a message to a specific MQTT topic
 * 
 * This function sends a message (payload) to the specified topic on the MQTT broker. 
 * It requires an active MQTT connection established via `mqtt_start`.
 * 
 * @param[in] topic The MQTT topic to which the message will be published
 * @param[in] payload The message content to publish (must be a null-terminated string)
 * @return 
 *      - ESP_OK on successful message publication
 *      - ESP_FAIL on failure to publish the message
 */
esp_err_t mqtt_publish(const char *topic, const char *payload);

/**
 * @brief Disconnect the MQTT client from the broker
 * 
 * This function gracefully disconnects the MQTT client from the broker and 
 * cleans up associated resources. It should be called when MQTT operations 
 * are no longer needed.
 * 
 * @return 
 *      - ESP_OK on successful disconnection
 *      - ESP_FAIL on failure to disconnect
 */
esp_err_t mqtt_disconnect();

#endif // _MQTT_SERVER_H_
