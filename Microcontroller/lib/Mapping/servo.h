#ifndef _SERVO_H_
#define _SERVO_H_

#include "esp_err.h"


#define SERVO_MAX_SPEED_CW      900
#define SERVO_MEDIUM_SPEED_CW   1050       
#define SERVO_LOW_SPEED_CW      1200    //TESTED

#define SERVO_STOP              1500

#define SERVO_LOW_SPEED_CCW     1800    //TESTED
#define SERVO_MEDIUM_SPEED_CCW  1950    
#define SERVO_MAX_SPEED_CCW     2100    





// Prototipos de funciones
esp_err_t servo_initialize(void);
esp_err_t servo_start(void);
esp_err_t servo_stop(void);
int16_t readAngle(void);
void servo_set_speed(char*);
void servo_invert(void);



#endif