/*  WiFi softAP Example */
#include "ap_server.h"
#include <esp_log.h>
static const char *TAG = "wifi softAP";
//static const UBaseType_t taskPriority = 1;

void vTask1(void *pvParameters)
{
    for (;;)
    {
        // Display the core on which the task is running
        printf("Task1 is running on core %d\n", xPortGetCoreID());
        // Wait 3 seconds
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station %02X:%02X:%02X:%02X:%02X:%02X join, AID=%d",
         event->mac[0], event->mac[1], event->mac[2], event->mac[3], event->mac[4], event->mac[5], event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station %02X:%02X:%02X:%02X:%02X:%02X join, AID=%d",
         event->mac[0], event->mac[1], event->mac[2], event->mac[3], event->mac[4], event->mac[5], event->aid);
    }
}

esp_err_t wifi_init_softap(void)
{
    esp_err_t err = esp_netif_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_netif_init failed.");
        return err;
    }

    esp_err_t event_err = esp_event_loop_create_default();
    if (event_err != ESP_OK) {
        ESP_LOGE(TAG, "Default event loop creation failed.");
        return event_err;
    }

    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    esp_err_t wifi_init_err = esp_wifi_init(&cfg);
    if (wifi_init_err != ESP_OK) {
        ESP_LOGE(TAG, "WiFi Allocate failed.");
        return wifi_init_err;
    }

    esp_err_t event_handler_err = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);
    if (event_handler_err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register an instance of event handler to the default loop.");
        return event_handler_err;
    }

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = ESP_WIFI_SSID,
            .ssid_len = strlen(ESP_WIFI_SSID),
            .channel = ESP_WIFI_CHANNEL,
            .password = ESP_WIFI_PASS,
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    esp_err_t wifi_mode_err = esp_wifi_set_mode(WIFI_MODE_AP);
    if (wifi_mode_err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set the WiFi operating mode.");
        return wifi_mode_err;
    }

    esp_err_t wifi_config_err = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    if (wifi_config_err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set the configuration of the AP.");
        return wifi_config_err;
    }
    
    esp_err_t wifi_start_err = esp_wifi_start();
    if (wifi_start_err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start WiFi.");
        return wifi_start_err;
    }

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
            ESP_WIFI_SSID, ESP_WIFI_PASS, ESP_WIFI_CHANNEL);
    
    return ESP_OK;
}

/*
void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    TaskHandle_t xHandle1 = NULL;
    xTaskCreatePinnedToCore(vTask1,       // Entry function of the task
                            "Task1",      // Name of the task
                            10000,        // The number of words to allocate for use as the task's stack (arbitrary size enough for this task)
                            NULL,         // No parameter passed to the task
                            taskPriority, // Priority of the task
                            &xHandle1,    // Handle to the created task
                            0);           // Task must be executed on core 0

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
}*/