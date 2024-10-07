#include "esp_err.h"
#include "motors.h"
#include "lights.h"

#include "servomotor.h"

esp_err_t initializa_system(){
  lights_init();
  esp_err_t err = servomotor_init();
  if(err != ESP_OK) return err;
  return ESP_OK;
}

void app_main()
{

  if(initializa_system() != ESP_OK) {
    abort();
  }
  /*
  lights_init();
  while (1)
  {
    error_led_on();
    vTaskDelay(pdMS_TO_TICKS(5000)); // Gira durante 2 segundos
    error_led_off();
    vTaskDelay(pdMS_TO_TICKS(5000)); // Gira durante 2 segundos
  }
  */
}
