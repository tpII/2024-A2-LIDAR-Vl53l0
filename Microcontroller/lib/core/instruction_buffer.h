#ifndef _INSTRUCTION_BUFFER_H_
#define _INSTRUCTION_BUFFER_H_

#include "esp_err.h"

esp_err_t getInstruction(char *);

esp_err_t saveInstruction(char *);

#endif