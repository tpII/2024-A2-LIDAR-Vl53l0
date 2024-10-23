#ifndef _FREERTOS_H_
#define _FREERTOS_H_

#include <esp_err.h>
#include <esp_log.h>
#include "ap_server.h"
// #include "mqtt_server.h"

esp_err_t initialize_access_point(void);
esp_err_t initialize_mqtt_server(void);
esp_err_t complete_initialization(void);
esp_err_t abort_tasks(void);

#endif