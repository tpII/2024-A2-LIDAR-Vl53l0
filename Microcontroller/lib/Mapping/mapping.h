#ifndef _MAPPING_H_
#define _MAPPING_H_

#include "esp_err.h"

esp_err_t mapping_init(void);
esp_err_t getMappingValue(uint16_t *, uint16_t *);
esp_err_t mapping_pause(void);
esp_err_t mapping_stop(void);
esp_err_t mapping_restart(void);

#endif