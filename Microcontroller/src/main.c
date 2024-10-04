#include "motors.h"
#include "lights.h"

// Tarea para controlar los motores
void motor_control_task(void *arg)
{
  while (1)
  {
    motors_command(FORWARD);
    vTaskDelay(pdMS_TO_TICKS(2000)); // Gira durante 2 segundos

    motors_command(STOP);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo

    motors_command(BACKWARD);
    vTaskDelay(pdMS_TO_TICKS(2000)); // Gira durante 2 segundos

    motors_command(STOP);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo

    motors_command(FORWARD);
    vTaskDelay(pdMS_TO_TICKS(2000)); // Gira durante 2 segundos

    motors_command(STOP);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo

    motors_command(BACKWARD);
    vTaskDelay(pdMS_TO_TICKS(2000)); // Gira durante 2 segundos

    motors_command(STOP);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo
  }
}

void app_main()
{
  /*motors_setup();  // Inicializar GPIO
  xTaskCreate(motor_control_task, "motor_control_task", 2048, NULL, 1, NULL);  // Crear tarea de control
  */
  lights_init();
  while (1)
  {
    error_led_on();
    vTaskDelay(pdMS_TO_TICKS(5000)); // Gira durante 2 segundos
    error_led_off();
    vTaskDelay(pdMS_TO_TICKS(5000)); // Gira durante 2 segundos
  }
}
