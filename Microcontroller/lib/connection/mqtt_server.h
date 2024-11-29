#ifndef _MQTT_SERVER_H_
#define _MQTT_SERVER_H_

#include "esp_err.h"

// Inicializa el cliente MQTT y se conecta al broker
esp_err_t mqtt_start();

// Publica un mensaje en un tópico específico
esp_err_t mqtt_publish(const char *topic, const char *payload);

// Desconecta el cliente MQTT
esp_err_t mqtt_disconnect();

#endif