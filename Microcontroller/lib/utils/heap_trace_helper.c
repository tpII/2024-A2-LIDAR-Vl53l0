#include "heap_trace_helper.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define TAG "HEAP_TRACE"

static heap_trace_record_t trace_record[NUM_RECORDS];

esp_err_t start_heap_trace(void)
{
    esp_err_t err = heap_trace_init_standalone(trace_record, NUM_RECORDS);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing heap trace: %s", esp_err_to_name(err));
        return err;
    }

    err = heap_trace_start(HEAP_TRACE_LEAKS);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error starting heap trace: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Heap tracing started successfully");
    return ESP_OK;
}

void stop_heap_trace(void)
{
    esp_err_t err = heap_trace_stop();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error stopping heap trace: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "Heap tracing stopped. Dumping trace records:");
    heap_trace_dump();
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
