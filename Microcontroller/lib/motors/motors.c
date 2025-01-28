#include "motors.h"

MOTOR_STRUCT MOTOR1 = {MOTOR1_PIN1, MOTOR1_PIN2};
MOTOR_STRUCT MOTOR2 = {MOTOR2_PIN1, MOTOR2_PIN2};

// Función para inicializar los pines
void motors_setup() {
    gpio_config_t io_conf;

    // Configurar los pines de los motores como salidas
    io_conf.intr_type = GPIO_INTR_DISABLE;  // Deshabilitar interrupciones
    io_conf.mode = GPIO_MODE_OUTPUT;        // Configurar como salida
    io_conf.pin_bit_mask = (1ULL << MOTOR1_PIN1) | (1ULL << MOTOR1_PIN2) |
                           (1ULL << MOTOR2_PIN1) | (1ULL << MOTOR2_PIN2);  // Máscara de pines
    io_conf.pull_down_en = 0;               // Deshabilitar pull-down
    io_conf.pull_up_en = 0;                 // Deshabilitar pull-up
    gpio_config(&io_conf);                  // Aplicar configuración

    // Asegurarse de que los pines de los motores estén en estado bajo al inicio (apagados)
    gpio_set_level(MOTOR1_PIN1, 0);
    gpio_set_level(MOTOR1_PIN2, 0);
    gpio_set_level(MOTOR2_PIN1, 0);
    gpio_set_level(MOTOR2_PIN2, 0);
}

// Función controladora de los motores
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
    case ROTATE_LEFT:
        gpio_set_level(MOTOR1.PIN1, 1);
        gpio_set_level(MOTOR1.PIN2, 0);
        gpio_set_level(MOTOR2.PIN1, 0);
        gpio_set_level(MOTOR2.PIN2, 0);
        break;
    case ROTATE_RIGHT:
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


