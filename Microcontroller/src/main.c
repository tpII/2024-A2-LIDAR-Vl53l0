#include "esp_log.h"
#include "cyclops_core.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "instruction_buffer.h"
#include <stdio.h>
#include <string.h>
#include "mqtt_handler.h"
static const char *TAG = "MAIN";

void app_main(void)
{
        esp_err_t err = ESP_FAIL;
        err = system_init();
        if (err != ESP_OK)
        {
                ESP_LOGE(TAG, "FATAL ERROR");
        }
        else
        {
                err = createTasks();
                if (err != ESP_OK)
                {
                        ESP_LOGE(TAG, "FATAL ERROR");
                }
        }

        ESP_LOGI(TAG, "ALL DONE");

        // TESTING SAVING INST
        /*
        char inst1[] = "TESTING SEMDING MESSAGE";

            while (1)
        {
                err = sendInfoMesage(TAG,inst1);
                if (err != ESP_OK)
                {
                        ESP_LOGW(TAG, "MESSAGE SEND FAIL: %s", esp_err_to_name(err));
                } else {
                        ESP_LOGI(TAG,"MESSAGE SEND");
                }
                vTaskDelay(1500 / portTICK_PERIOD_MS);
        }
*/
}
