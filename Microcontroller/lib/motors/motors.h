#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Definición de pines para los motores
#define MOTOR1_PIN1 GPIO_NUM_23  // Pin de control para Motor 1 (dirección 1)
#define MOTOR1_PIN2 GPIO_NUM_22  // Pin de control para Motor 1 (dirección 2)
#define MOTOR2_PIN1 GPIO_NUM_21  // Pin de control para Motor 2 (dirección 1)
#define MOTOR2_PIN2 GPIO_NUM_19  // Pin de control para Motor 2 (dirección 2)

typedef struct {
    gpio_num_t PIN1;
    gpio_num_t PIN2;
} MOTOR_STRUCT;

typedef enum {
    FORWARD,
    BACKWARD,
    ROTATE_RIGHT,
    ROTATE_LEFT,
    STOP
} DIRECTIONS;

MOTOR_STRUCT MOTOR1 = {MOTOR1_PIN1, MOTOR1_PIN2};
MOTOR_STRUCT MOTOR2 = {MOTOR2_PIN1, MOTOR2_PIN2};