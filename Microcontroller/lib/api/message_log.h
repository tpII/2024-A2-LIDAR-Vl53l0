#ifndef _MESSAGE_LOG_H_
#define _MESSAGE_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 1000  // revisar

void get_report_log(void);
uint8_t add_date_to_log(void);
uint8_t write_message(char *msg);

#endif 