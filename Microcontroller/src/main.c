#include "esp_log.h"
#include "cyclops_core.h"

static const char *TAG = "MAIN";

void app_main(void)
{
        if (system_init() != ESP_OK)
        {
                ESP_LOGE(TAG, "FATAL ERROR");
        }

        if (createTasks() != ESP_OK)
        {
                ESP_LOGE(TAG, "FATAL ERROR");
        }

        ESP_LOGI(TAG, "ALL DONE");


        
}
