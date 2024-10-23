#include "freertos.h"

static const char *TAG = "FreeRTOS";

// tasks
static void access_point(void *pvParameters);
static void mqtt_server(void *pvParameters);
static void abort(void *pvParameters);

// handlers
TaskHandle_t ap_handler;
TaskHandle_t mqtt_handler;
TaskHandle_t abort_handler;

esp_err_t complete_initialization(void){
    esp_err_t err = initialize_access_point();
    if (err != ESP_OK) {
        return ESP_FAIL;
    }

    err = initialize_mqtt_server();
    if (err != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

// create tasks
esp_err_t initialize_access_point(void)
{
    xTaskCreatePinnedToCore(access_point, "AP Server", 4096, NULL, 1, &ap_handler, 0);
    return ESP_OK;
}

esp_err_t initialize_mqtt_server(void){
    xTaskCreatePinnedToCore(mqtt_server, "MQTT Server", 4096, NULL, 1, NULL, 0);
    return ESP_OK;
}

// tasks
static void access_point(void *pvParameters)
{
    esp_err_t err = initialize_server();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize AP server: err %d", err);
    }
}

static void mqtt_server(void *pvParameters)
{
    esp_err_t err; // = mqtt_connect(); // realizar merge para poder utilizar funcion
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize MQTT server: err %d", err);
    }
}

esp_err_t abort_tasks(void)
{
    xTaskCreatePinnedToCore(abort, "Abort Tasks", 4096, NULL, 1, &abort_handler, 0);
    return ESP_OK;
}

static void abort(void *pvParameters)
{
    if (mqtt_handler == NULL) {
        // enviar error al report log
        goto EXIT;
    }
    else {
        vTaskDelete(mqtt_handler);
    }

    if (ap_handler == NULL) {
        // enviar error al report log 
        goto EXIT;
    }
    else {
        vTaskDelete(ap_handler);
    }

    EXIT:
        vTaskDelete(NULL);
}