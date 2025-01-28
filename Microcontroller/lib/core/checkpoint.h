#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_

#include "esp_err.h"
#include "esp_log.h"
#include "cyclops_core.h"

esp_err_t setCheckpoint(void);

void activeCheckpoint(void);

#endif