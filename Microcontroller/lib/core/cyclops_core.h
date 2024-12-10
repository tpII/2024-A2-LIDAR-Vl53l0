#ifndef _CYCLOPS_CORE_H_
#define _CYCLOPS_CORE_H_

#include "esp_err.h"

esp_err_t system_init(void);
esp_err_t createTasks(void);
void abort_tasks(void);

#endif