#ifndef _FREERTOS_H_
#define _FREERTOS_H_

#include <esp_err.h>
#include <esp_log.h>
#include "ap_server.h"
#include "http_server.h"

esp_err_t initialize_access_point(void);
esp_err_t initialize_http_server(void);

#endif