#ifndef _SERVOMOTOR_H_
#define _SERVOMOTOR_H_

#include "esp_err.h"

#define SERVO_MIN_PULSEWIDTH_US    900          //0.5 ms
#define SERVO_MAX_PULSEWIDTH_US    2500         //2.5 ms
#define SERVO_MIN_DEGREE           0            //JUST FOR TEST
#define SERVO_MAX_DEGREE           360          //JUST FOR TEST

#define SERVO_PULSE_GPIO                4              //GPIO connects to PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ    1000000       //1MHz, 1us per tick 
#define SERVO_TIMEBASE_PERIOD           20000           //20000 ticks, 20ms

esp_err_t servomotor_init(void);
esp_err_t servomotor_set_angle(uint16_t);

#endif