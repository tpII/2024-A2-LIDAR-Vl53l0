#ifndef _CYCLOPS_CORE_H_
#define _CYCLOPS_CORE_H_

#include "esp_err.h"
#include "http_handler.h"

esp_err_t system_init(void);
esp_err_t createTasks(void);
void abort_tasks(void);

#endif