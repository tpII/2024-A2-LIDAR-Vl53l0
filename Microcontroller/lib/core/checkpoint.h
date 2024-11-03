#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_

#include "esp_err.h"

esp_err_t setCheckpoint(void);

void activeCheckpoint(void);

#endif