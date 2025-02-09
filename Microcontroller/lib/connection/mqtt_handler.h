/**
 * @file mqtt_handler.h
 * @author 
 *      Guerrico Leonel (lguerrico@outlook.com)
 * @brief Interface for MQTT message handling
 * 
 * This header defines the functions used to interact with the MQTT service. 
 * It includes functionality for sending structured data, such as control messages, 
 * mapping values, and battery levels, as well as for receiving instructions via MQTT.
 * 
 * @version 1.0
 * @date 2024-12-04
 * 
 */

 #ifndef _MQTT_HANDLER_H_
 #define _MQTT_HANDLER_H_
 
 #include "esp_err.h"
 
 /**
  * @brief Retrieve an instruction message via MQTT
  * 
  * This function retrieves an instruction to execute and stores it in the 
  * provided `inst` parameter by reference.
  * 
  * @param[out] inst Pointer to a buffer where the instruction will be stored
  * @return
  *      - ESP_OK on success
  *      - ESP_FAIL on failure
  */
 esp_err_t getInstructionMessage(char *inst);
 
 /**
  * @brief Send a mapping value (distance and angle) via MQTT
  * 
  * This function sends a tuple containing a distance and an angle as a 
  * JSON-encoded MQTT message.
  * 
  * @param[in] distance Distance value in centimeters
  * @param[in] angle Angle value in degrees
  * @return 
  *      - ESP_OK on success
  *      - ESP_FAIL on failure
  */
 esp_err_t sendMappingValue(uint16_t distance, int16_t angle);
 
 /**
  * @brief Send the battery charge percentage via MQTT
  * 
  * Sends the current battery charge level to the MQTT broker as a 
  * JSON-encoded message.
  * 
  * @param[in] batteryLevel Battery charge percentage (0-100)
  * @return 
  *      - ESP_OK on success
  *      - ESP_FAIL on failure
  */
 esp_err_t sendBatteryLevel(uint8_t batteryLevel);
 
 /**
  * @brief Send an error message via MQTT
  * 
  * Sends an error message with the specified tag and message content 
  * to the MQTT broker.
  * 
  * @param[in] TAG Label identifying the source of the message
  * @param[in] msg Error message content
  * @return 
  *      - ESP_OK on success
  *      - ESP_FAIL on failure
  */
 esp_err_t sendErrorMessage(const char *TAG, const char *msg);
 
 /**
  * @brief Send an informational message via MQTT
  * 
  * Sends an informational message with the specified tag and content 
  * to the MQTT broker.
  * 
  * @param[in] TAG Label identifying the source of the message
  * @param[in] msg Informational message content
  * @return 
  *      - ESP_OK on success
  *      - ESP_FAIL on failure
  */
 esp_err_t sendInfoMessage(const char *TAG, const char *msg);
 
 /**
  * @brief Send a warning message via MQTT
  * 
  * Sends a warning message with the specified tag and content to the 
  * MQTT broker.
  * 
  * @param[in] TAG Label identifying the source of the message
  * @param[in] msg Warning message content
  * @return 
  *      - ESP_OK on success
  *      - ESP_FAIL on failure
  */
 esp_err_t sendWarningMessage(const char *TAG, const char *msg);
 
 
 #endif // _MQTT_HANDLER_H_
 