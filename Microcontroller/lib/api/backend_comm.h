#ifndef BACKEND_COMM_H
#define BACKEND_COMM_H

#include <stdio.h>
#include <esp_err.h>
//#include "mqtt_server.h"

// Inicialización de conexión al backend
esp_err_t backend_init(void);

// Enviar mensaje al backend
esp_err_t send_to_backend(const char *topic, const char *message);

// Finalizar la conexión
esp_err_t backend_disconnect();

#endif // BACKEND_COMM_H
