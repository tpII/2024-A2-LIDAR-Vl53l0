#include "ap_server.h"

static const char *TAG = "wifi softAP";
static SemaphoreHandle_t client_connected_semaphore;

static void wifi_event_handler(void*, esp_event_base_t , int32_t , void* );


/**
 * @brief WiFi Event Handler for managing station connections and IP assignment.
 * 
 * This function handles various WiFi events such as station connecting and disconnecting,
 * as well as IP assignment for connected clients. It logs the MAC address and AID of 
 * the connected or disconnected stations and signals when an IP is assigned to a client.
 * 
 * @param[in] arg Pointer to the argument passed to the event handler (usually NULL).
 * @param[in] event_base Event base identifier.
 * @param[in] event_id Specific event ID to handle.
 * @param[in] event_data Data associated with the event, such as MAC address and AID.
 * 
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

esp_err_t wait_for_client_connection() {
    ESP_LOGI(TAG, "Esperando a que un dispositivo se conecte...");
    // Bloquea hasta que el semáforo sea liberado por la conexión de un dispositivo
    xSemaphoreTake(client_connected_semaphore, portMAX_DELAY);
    ESP_LOGI(TAG, "¡Dispositivo conectado!");
    return ESP_OK;
}