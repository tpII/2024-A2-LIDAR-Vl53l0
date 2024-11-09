#ifndef BACKEND_COMM_H
#define BACKEND_COMM_H

#include <stdio.h>
#include <esp_err.h>
#include "mqtt_server.h"
#include "mqtt_handler.h"

#include <cJSON.h>

// Enviar mensaje al backend
esp_err_t send_message_to_backend(const char* ESP_TAG, const char *msg, const char *msg_type);

#endif // BACKEND_COMM_H
