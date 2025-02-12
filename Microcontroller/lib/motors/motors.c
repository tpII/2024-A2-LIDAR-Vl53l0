/**
 * @file motors.c
 * @author Octavio Perez Balcedo
 * @brief Motor control implementation for ESP32.
 * 
 * This file contains the implementation of functions to control two DC motors 
 * using GPIO on an ESP32. The motors can move forward, backward, rotate in place, 
 * or stop, based on the given commands.
 * 
 * @date 2025-02-09
 * 
 */
#include "motors.h"

// Global motor structures for easy control
MOTOR_STRUCT MOTOR1 = {MOTOR1_PIN1, MOTOR1_PIN2};
MOTOR_STRUCT MOTOR2 = {MOTOR2_PIN1, MOTOR2_PIN2};

/**
 * @brief Initializes the motor control GPIOs.
 * 
 * Configures the motor control pins as output and ensures they 
 * start in a low state to prevent unintended movement.
 */
void motors_setup() {
    gpio_config_t io_conf;

    // Configure motor pins as outputs
    io_conf.intr_type = GPIO_INTR_DISABLE;  // Deshabilitar interrupciones
    io_conf.mode = GPIO_MODE_OUTPUT;        // Configurar como salida
    io_conf.pin_bit_mask = (1ULL << MOTOR1_PIN1) | (1ULL << MOTOR1_PIN2) |
                           (1ULL << MOTOR2_PIN1) | (1ULL << MOTOR2_PIN2);  // Máscara de pines
    io_conf.pull_down_en = 0;               // Deshabilitar pull-down
    io_conf.pull_up_en = 0;                 // Deshabilitar pull-up
    gpio_config(&io_conf);                  // Aplicar configuración

    // Ensure motors are initially off
    gpio_set_level(MOTOR1_PIN1, 0);
    gpio_set_level(MOTOR1_PIN2, 0);
    gpio_set_level(MOTOR2_PIN1, 0);
    gpio_set_level(MOTOR2_PIN2, 0);
}

/**
 * @brief Controls the motors based on the given direction.
 * 
 * Adjusts the GPIO output levels to move the motors in the specified direction.
 * 
 * @param[in] D The movement direction.
 */
void motors_command(DIRECTIONS D){
    switch (D){
    case FORWARD:
        gpio_set_level(MOTOR1.PIN1, 1);
        gpio_set_level(MOTOR1.PIN2, 0);
        gpio_set_level(MOTOR2.PIN1, 1);
        gpio_set_level(MOTOR2.PIN2, 0);
        break;
    case BACKWARD:
        gpio_set_level(MOTOR1.PIN1, 0);
        gpio_set_level(MOTOR1.PIN2, 1);
        gpio_set_level(MOTOR2.PIN1, 0);
        gpio_set_level(MOTOR2.PIN2, 1);
        break;
    case ROTATE_RIGHT:
        gpio_set_level(MOTOR1.PIN1, 1);
        gpio_set_level(MOTOR1.PIN2, 0);
        gpio_set_level(MOTOR2.PIN1, 0);
        gpio_set_level(MOTOR2.PIN2, 0);
        break;
    case ROTATE_LEFT:
        gpio_set_level(MOTOR1.PIN1, 0);
        gpio_set_level(MOTOR1.PIN2, 0);
        gpio_set_level(MOTOR2.PIN1, 1);
        gpio_set_level(MOTOR2.PIN2, 0);
        break;
    case STOP:
        gpio_set_level(MOTOR1.PIN1, 0);
        gpio_set_level(MOTOR1.PIN2, 0);
        gpio_set_level(MOTOR2.PIN1, 0);
        gpio_set_level(MOTOR2.PIN2, 0);
        break;
    default:
        gpio_set_level(MOTOR1.PIN1, 0);
        gpio_set_level(MOTOR1.PIN2, 0);
        gpio_set_level(MOTOR2.PIN1, 0);
        gpio_set_level(MOTOR2.PIN2, 0);
        break;
    }
}


