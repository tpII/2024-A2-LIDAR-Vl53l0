#ifndef _JSON_HELPER_H_
#define _JSON_HELPER_H_
#include "cJSON.h"
#include "esp_err.h"

esp_err_t create_json_data(char **, const char **, const char **, const size_t);
esp_err_t deserealize_json_data(const char *,char *, const size_t);
void print_json_data(const char *);

#endif