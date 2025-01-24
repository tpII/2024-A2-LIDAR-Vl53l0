#include "esp_log.h"
#include "cyclops_core.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mapping.h"
#include "battery.h"
#include "freeRtosTasks.h"
#include "motors.h"

static const char *TAG = "MAIN";

void app_main(void)
{
        esp_err_t err;

        ESP_LOGI(TAG, "Iniciando Battery Service...");
        err = battery_sensor_init();
        if (err != ESP_OK)
        {
                ESP_LOGE(TAG, "Error Iniciando Battery:  %s", esp_err_to_name(err));
                return 1;
        }
        ESP_LOGI(TAG, "Battery Service Iniciado!");

        ESP_LOGI(TAG, "Iniciando Mapping Service...");
        err = mapping_init();
        if (err != ESP_OK)
        {
                ESP_LOGE(TAG, "Error Iniciando Mapping:  %s", esp_err_to_name(err));
                return 1;
        }
        ESP_LOGI(TAG, "Mapping Service Iniciado!");

        ESP_LOGI(TAG, "Iniciando Tareas...");
        err = createTasks();
        if (err != ESP_OK)
        {
                ESP_LOGE(TAG, "Error Iniciando Tareas:  %s", esp_err_to_name(err));
                return 1;
        }
        ESP_LOGI(TAG, "Tareas Iniciadas!");

        ESP_LOGI(TAG, "Iniciando Motores Service...");
        motors_setup();
        ESP_LOGI(TAG, "Motores Service Iniciado!");

        uint16_t angle = 0;
        uint16_t distance = 0;
        uint8_t battery_level = 0;
        while (1)
        {       
                ESP_LOGW(TAG, "Going FORWARD");
                motors_command(FORWARD);
                vTaskDelay(200);
                ESP_LOGW(TAG, "Going BACKWARD");
                motors_command(BACKWARD);
                vTaskDelay(200);

                err = getMappingValue(&angle,&distance);
                if(err != ESP_OK){
                        ESP_LOGE(TAG,"FAIL TO GET MAPPING VALUE");
                }
                ESP_LOGW(TAG,"Mapping Values: %d° - %dmm",angle,distance);

                err = battery_sensor_read(&battery_level);
                if(err != ESP_OK){
                        ESP_LOGE(TAG,"FAIL TO GET BATTERY VALUE");
                }
                ESP_LOGW(TAG,"Battery Level: %u",battery_level);
                ESP_LOGI(TAG, "------------------------------------");
                vTaskDelay(200);
        }

        ESP_LOGE(TAG, "END OF MAIN...");
        return 0;
}
