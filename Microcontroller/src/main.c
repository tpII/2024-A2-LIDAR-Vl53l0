#include "esp_log.h"
#include "cyclops_core.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "instruction_buffer.h"
#include <stdio.h>
#include <string.h>
static const char *TAG = "MAIN";

void app_main(void)
{
        esp_err_t err = ESP_FAIL;
        err = system_init();
        if (err != ESP_OK)
        {
                ESP_LOGE(TAG, "FATAL ERROR");
        }
        else{
                err = createTasks();
                if (err != ESP_OK)
                {
                        ESP_LOGE(TAG, "FATAL ERROR");
                }
        }
        
        ESP_LOGI(TAG, "ALL DONE");

        // TESTING SAVING INST
        char inst1[] = "SpeedUp";
        char inst2[] = "SpeedDown";
        uint8_t i = 0;
        while (i < 30)
        {
                /*if (i % 2 == 0)
                {
                        if (saveInstruction(inst1) != ESP_OK)
                        {
                                ESP_LOGW(TAG, "ERROR SAVING 1");
                        }
                }
                else
                {
                        if (saveInstruction(inst2) != ESP_OK)
                        {
                                ESP_LOGW(TAG, "ERROR SAVING 2");
                        }
                }
                */
                i++;
                vTaskDelay(100);
        }
}
