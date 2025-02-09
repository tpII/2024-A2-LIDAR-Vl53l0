/**
 * @file motors.h
 * @author Octavio Perez Balcedo
 *     
 * @brief Header file for motor control functions.
 * 
 * This file contains the definitions and function declarations 
 * for controlling two DC motors using GPIO on an ESP32.
 */


#ifndef _MOTORS_H_
#define _MOTORS_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Motor control pin definitions
#define MOTOR1_PIN1 GPIO_NUM_19   /**< Motor 1 control pin (direction 1) */
#define MOTOR1_PIN2 GPIO_NUM_18   /**< Motor 1 control pin (direction 2) */
#define MOTOR2_PIN1 GPIO_NUM_5    /**< Motor 2 control pin (direction 1) */
#define MOTOR2_PIN2 GPIO_NUM_17   /**< Motor 2 control pin (direction 2) */

/**
 * @brief Structure for motor GPIO pin configuration.
 * 
 * This structure holds the two GPIO pins used to control 
 * the direction of a single motor.
 */
typedef struct {
    gpio_num_t PIN1;
    gpio_num_t PIN2;
} MOTOR_STRUCT;

/**
 * @brief Enumeration for motor movement directions.
 * 
 * Defines possible movement commands for the motors.
 */
typedef enum {
    FORWARD,
    BACKWARD,
    ROTATE_RIGHT,
    ROTATE_LEFT,
    STOP
} DIRECTIONS;

/**
 * @brief Initializes the motor control pins.
 * 
 * Configures the GPIO pins for motor control as outputs 
 * and ensures they are initially set to low (stopped).
 */
void motors_setup(void);

/**
 * @brief Commands the motors to move in a specified direction.
 * 
 * Controls the GPIO pins to achieve movement in the desired direction.
 * 
 * @param[in] D The direction in which the motors should move.
 */
void motors_command(DIRECTIONS D);

#endif