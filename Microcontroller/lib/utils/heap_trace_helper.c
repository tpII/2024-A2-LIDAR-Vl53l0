/**
 * @file heap_trace_helper.c
 * @author Octavio Perez Balcedo
 * @brief Implementation of heap tracing for memory leak detection.
 * 
 * This file implements functions to start and stop heap tracing
 * in LEAKS mode. It helps identify memory leaks by tracking dynamic
 * allocations in an ESP32 application.
 * 
 * @date 2025-02-09
 * @version 1.0
 */
#include "heap_trace_helper.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "debug_helper.h"

#define TAG "HEAP_TRACE"

static heap_trace_record_t trace_record[NUM_RECORDS];   /**< Buffer for heap trace records */

/**
 * @brief Starts heap tracing in LEAKS mode.
 * 
 * Initializes the heap trace system with a standalone buffer and starts tracking
 * heap allocations that are not freed.
 * 
 * @return esp_err_t ESP_OK if successful, otherwise an error code.
 */
esp_err_t start_heap_trace()
{
    esp_err_t err;
    
    // Initialize heap trace with the standalone buffer
    err = heap_trace_init_standalone(trace_record, NUM_RECORDS);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing heap trace: %s", esp_err_to_name(err));
        LOG_MESSAGE_E(TAG,"Error initializing heap trace");
        return err;
    }

    // Start heap trace in LEAKS mode
    err = heap_trace_start(HEAP_TRACE_LEAKS);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error starting heap trace: %s", esp_err_to_name(err));
        LOG_MESSAGE_E(TAG,"Error starting heap trace");
        return err;
    }

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Heap tracing started successfully"));
    return ESP_OK;
}

/**
 * @brief Stops heap tracing and outputs the recorded heap allocations.
 * 
 * Stops heap trace logging and prints all recorded memory allocations that
 * were not freed, helping detect memory leaks.
 */
void stop_heap_trace()
{
    // Stop heap trace
    esp_err_t err = heap_trace_stop();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error stopping heap trace: %s", esp_err_to_name(err));
        LOG_MESSAGE_E(TAG,"Error stopping heap trace");
        return;
    }
    
    // Dump recorded heap allocations
    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Heap tracing stopped. Dumping trace records:"));
    heap_trace_dump();
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
