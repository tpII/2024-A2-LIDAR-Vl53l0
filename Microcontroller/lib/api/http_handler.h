#ifndef _HTTP_HANDLER_H_
#define _HTTP_HANDLER_H_

#include "http_client.h"

esp_err_t getInstruccionMessage(char *msg);

esp_err_t sendControlMessage(const char *msg, const size_t length);

esp_err_t sendSensorValue(const uint16_t distance, const uint16_t angle);

#endif //HTTP_HANDLER_H_
