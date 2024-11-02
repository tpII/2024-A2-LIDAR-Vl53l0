#ifndef _MQTT_HANDLER_H_
#define _MQTT_HANDLER_H_

#include "esp_err.h"

esp_err_t getInstruccionMessage(char *msg);

esp_err_t sendControlMessage(const char *msg, const size_t length);

esp_err_t sendMappingValue(const uint16_t distance, const uint16_t angle);

esp_err_t sendBatteryLevel(const uint8_t batteryLevel);

#endif //HTTP_HANDLER_H_
