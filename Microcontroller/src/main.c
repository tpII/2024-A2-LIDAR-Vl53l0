#include "esp_log.h"
#include "cyclops_core.h"

static const char *TAG = "MAIN";

int app_main(void)
{
        if (system_init() != ESP_OK)
        {
                return 1;
        }

        if (createTasks() != ESP_OK)
        {
                return 1;
        }

        return 0;
}
