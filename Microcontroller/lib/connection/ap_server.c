/**
 * @file ap_server.c
 * @author 
 *      Ossola Florencia (flor.ossola13@gmail.com)
 * @brief Implementation file for initializing and configuring the WiFi SoftAP server.
 * 
 * This file implements the necessary functions to configure and manage a WiFi SoftAP 
 * server on an ESP32 device. It handles event registration, WiFi initialization, 
 * client connections, and TCP server setup.
 * 
 * This implementation is based on examples provided by the ESP-IDF library, 
 * customized for this project’s specific requirements.
 * 
 * @version 1.0
 * @date 2024-12-09
 * 
 */
#include "ap_server.h"
#include "lights.h"

static const char *TAG = "wifi softAP";
static SemaphoreHandle_t client_connected_semaphore;

static void wifi_event_handler(void*, esp_event_base_t , int32_t , void* );

/**
 * @brief WiFi Event Handler for managing station connections and IP assignment.
 * 
 * Handles WiFi events, including when a station connects or disconnects, and 
 * when an IP address is assigned to a connected client. It logs station MAC addresses 
 * and AID (Association ID) for debugging purposes. When a client receives an IP, 
 * a semaphore is released to signal successful connection.
 * 
 * @param[in] arg Pointer to additional arguments (usually NULL).
 * @param[in] event_base Base event type (WiFi or IP).
 * @param[in] event_id Specific event identifier.
 * @param[in] event_data Pointer to event-specific data.
 */
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station %02X:%02X:%02X:%02X:%02X:%02X join, AID=%d",
         event->mac[0], event->mac[1], event->mac[2], event->mac[3], event->mac[4], event->mac[5], event->aid);
        // Libera el semáforo cuando se conecte un dispositivo
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station %02X:%02X:%02X:%02X:%02X:%02X join, AID=%d",
         event->mac[0], event->mac[1], event->mac[2], event->mac[3], event->mac[4], event->mac[5], event->aid);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_AP_STAIPASSIGNED) {
        ESP_LOGI(TAG, "IP asignada a cliente");
        xSemaphoreGive(client_connected_semaphore);
    }
}

/**
 * @brief Initializes the Wi-Fi SoftAP mode.
 * 
 * Configures the ESP32 as an Access Point (AP), registering event handlers and 
 * initializing the necessary components for WiFi functionality. It sets up the AP 
 * parameters such as SSID, password, channel, and authentication mode.
 * 
 * @return 
 *      - ESP_OK if initialization succeeds.
 *      - ESP_FAIL if an error occurs.
 */
esp_err_t wifi_init_softap(void){

    // Inicializa el semáforo binario
    client_connected_semaphore = xSemaphoreCreateBinary();

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

    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &wifi_event_handler, NULL, NULL);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = ESP_WIFI_SSID,
            .ssid_len = strlen(ESP_WIFI_SSID),
            .channel = ESP_WIFI_CHANNEL,
            .password = ESP_WIFI_PASS,
            .max_connection = MAX_NUM_CONNECTIONS,
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

/**
 * @brief Initializes the server and Wi-Fi SoftAP functionality.
 * 
 * This function initializes the Non-Volatile Storage (NVS) and sets up the Wi-Fi SoftAP.
 * If an issue is detected with NVS (such as no free pages or a new version found), the storage 
 * is erased and reinitialized. After NVS is successfully initialized, the function calls 
 * `wifi_init_softap()` to configure and start the SoftAP mode.
 * 
 * In case of an error, it logs an appropriate message and returns the error code.
 * 
 * @return
 *      - ESP_OK if initialization is successful
 *      - ESP_ERR_NVS_NO_FREE_PAGES if NVS has no free pages and requires reinitialization
 *      - ESP_ERR_NVS_NEW_VERSION_FOUND if a new version of NVS is found and requires erasing
 *      - ESP_FAIL if Wi-Fi initialization fails
 */
esp_err_t initialize_server(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
      return err;
    }

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    esp_err_t err2 = wifi_init_softap();
    if(err2 != ESP_OK){
        ESP_LOGE(TAG, "Error initializing WiFi Soft AP: %s", esp_err_to_name(err2));
        return err2;
    }
    return ESP_OK;
}

/**
 * @brief Waits for a client connection.
 * 
 * This function blocks execution until a client successfully connects to the SoftAP. 
 * It relies on a binary semaphore, which is released when an IP is assigned to a client.
 * 
 * @return 
 *      - ESP_OK if a client connects successfully.
 */

esp_err_t wait_for_client_connection() {
    ESP_LOGI(TAG, "Esperando a que un dispositivo se conecte...");
    // Bloquea hasta que el semáforo sea liberado por la conexión de un dispositivo
    led_blink(300);
    xSemaphoreTake(client_connected_semaphore, portMAX_DELAY);
    led_blink(300);
    ESP_LOGI(TAG, "¡Dispositivo conectado!");
    return ESP_OK;
}

