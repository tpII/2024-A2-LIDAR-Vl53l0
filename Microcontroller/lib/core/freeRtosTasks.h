#ifndef _FREERTOS_TASKS_H_
#define _FREERTOS_TASKS_H_

#include "esp_err.h"

esp_err_t createTasks(void);
void abort_tasks(void);

#endif