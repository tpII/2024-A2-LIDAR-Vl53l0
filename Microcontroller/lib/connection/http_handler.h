#ifndef _HTTP_HANDLER_H_
#define _HTTP_HANDLER_H_

#include "cJSON.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include <string.h>
#include <stdlib.h>
#include "esp_err.h"
#include <stdio.h>

esp_err_t getHTTPInstruccion(char *, size_t);

#endif