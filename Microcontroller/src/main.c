#include "http_client.h"
#include "i2c.h"
#include "lights.h"
#include "ap_server.h"
#include "http_server.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

void app_main(void) {

    esp_err_t err =  wifi_init_softap();
    if(err){
        ESP_LOGE(TAG, "Error ocurred trying to establish wifi AP: err %d", err);
    }
   
   esp_err_t err2 = initialize_server();
   if(err2){
     ESP_LOGE(TAG, "Error ocurred trying to initialize server: err %d", err);
   }
   
}