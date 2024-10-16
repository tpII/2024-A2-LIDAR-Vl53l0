#ifndef _MQTT_SERVER_H_
#define _MQTT_SERVER_H_

#include "esp_err.h"

// Inicializa el cliente MQTT y se conecta al broker
esp_err_t mqtt_connect();

// Publica un mensaje en un tópico específico
esp_err_t mqtt_publish(const char *topic, const char *payload);

// Se suscribe a un tópico específico
esp_err_t mqtt_subscribe(const char *topic);

// Desconecta el cliente MQTT
esp_err_t mqtt_disconnect();

#endif