#include "motors.h"

// Función para inicializar los pines
void setup_gpio() {
    gpio_pad_select_gpio(MOTOR1_PIN1);
    gpio_pad_select_gpio(MOTOR1_PIN2);
    gpio_pad_select_gpio(MOTOR2_PIN1);
    gpio_pad_select_gpio(MOTOR2_PIN2);

    gpio_set_direction(MOTOR1_PIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR1_PIN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR2_PIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR2_PIN2, GPIO_MODE_OUTPUT);
}

// Función controladora de los motores
void MOTOR_COMMANDS(DIRECTIONS D){
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

// Tarea para controlar los motores
void motor_control_task(void *arg) {
    while (1) {
        MOTOR_COMMANDS(FORWARD);
        vTaskDelay(pdMS_TO_TICKS(2000));  // Gira durante 2 segundos

        MOTOR_COMMANDS(STOP);
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo

        MOTOR_COMMANDS(BACKWARD);
        vTaskDelay(pdMS_TO_TICKS(2000));  // Gira durante 2 segundos

        MOTOR_COMMANDS(STOP);
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo

        MOTOR_COMMANDS(FORWARD);
        vTaskDelay(pdMS_TO_TICKS(2000));  // Gira durante 2 segundos

        MOTOR_COMMANDS(STOP);
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo

        MOTOR_COMMANDS(BACKWARD);
        vTaskDelay(pdMS_TO_TICKS(2000));  // Gira durante 2 segundos

        MOTOR_COMMANDS(STOP);
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo
    }
}

void app_main() {
    setup_gpio();  // Inicializar GPIO
    xTaskCreate(motor_control_task, "motor_control_task", 2048, NULL, 1, NULL);  // Crear tarea de control
}
