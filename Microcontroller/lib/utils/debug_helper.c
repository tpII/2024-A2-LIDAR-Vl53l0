#include "debug_helper.h"
#include "mqtt_handler.h"
#include "esp_log.h"

void LOG_MESSAGE(int level, char *TAG, char *fmt) {
    // char log_buffer[100];
    // va_list args;
    // va_start(args, fmt);
    // vsnprintf(log_buffer, sizeof(log_buffer), fmt, args);
    // va_end(args);

    esp_err_t err = ESP_OK;
    // if (level == LOG_ERROR) {
    //     err = sendErrorMesage(TAG, fmt);
    // } else if (level == LOG_WARNING) {
    //     err = sendWarningMesage(TAG, fmt);
    // } else if (level == LOG_INFO) {
    //     err = sendInfoMesage(TAG, fmt);
    // }

    switch(level){
        case LOG_ERROR:
            err = sendErrorMesage(TAG, fmt);
            break;
        case LOG_WARNING:
            err = sendWarningMesage(TAG, fmt);
            break;
        case LOG_INFO:
            err = sendInfoMesage(TAG, fmt);
            break;
    }

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error enviando mensaje de log: %s", fmt);
    }
}
