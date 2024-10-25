#include "esp_err.h"
#include "esp_log.h"
#include "motors.h"
#include "lights.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "servomotor.h"
#include "servomotor2.h"

char *TAG = "MAIN";

esp_err_t initializa_system()
{
  lights_init();
  // esp_err_t err = servomotor_init();
  esp_err_t err = initialize_servo();

  if (err != ESP_OK)
  {
    return err;
  }
  return ESP_OK;
}

void task()
{

  // uint16_t angles[] = {0, 45, 90, 135, 180, 225, 270, 315, 360};
  float angles[] = {0.0f, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f, 360.0f};
  float actualAngle = 0;
  uint8_t index = 0;
  while (1)
  {
    /*if (servomotor_set_angle(angles[index]) != ESP_OK)
    {
      ESP_LOGE(TAG, "ERROR TO SET SERVOMOTOR TO ANGLE: %u", angles[index]);
    }
    else
    {
      ESP_LOGI(TAG, "SUCCESS TO SET SERVOMOTOR TO ANGLE: %u", angles[index]);
    }*/
    if (write_angle(angles[index]) != ESP_OK)
    {
      ESP_LOGE(TAG, "ERROR TO SET SERVOMOTOR TO ANGLE: %f", angles[index]);
    }
    else
    {
      ESP_LOGI(TAG, "SUCCESS TO SET SERVOMOTOR TO ANGLE: %f", angles[index]);
    }
    vTaskDelay(pdMS_TO_TICKS(4500));
    if (read_angle(&actualAngle) != ESP_OK)
    {
      ESP_LOGE(TAG, "ERROR READING ANGLE");
    }
    else
    {
      if (actualAngle != angles[index])
      {
        ESP_LOGE(TAG, "ERROR The angle read does not match the expected one, Angle: %f - Expected %f", actualAngle, angles[index]);
      }
      else
      {
        ESP_LOGE(TAG, "ERROR The angle read match the expected one, Angle: %f", actualAngle);
      }
    }

    if (++index > 8)
    {
      index = 0;
    }
  }
}

void app_main()
{

  if (initializa_system() != ESP_OK)
  {
    ESP_LOGE(TAG, "System initialization failed. Aborting...");
    abort();
  }

  vTaskDelay(pdMS_TO_TICKS(500));
  task();
}
