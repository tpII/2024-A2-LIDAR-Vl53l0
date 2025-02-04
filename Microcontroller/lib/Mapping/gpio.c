// #include "gpio.h"

// #define GPIO_XSHUT_FIRST GPIO_NUM_23 // Cambia esto al número de pin que desees
// // #define GPIO_XSHUT_SECOND GPIO_NUM_NC // Cambia esto al número de pin que desees
// // #define GPIO_XSHUT_THIRD GPIO_NUM_NC // Cambia esto al número de pin que desees

// static const char *TAG = "GPIO";

// esp_err_t gpio_init(void)
// {
//     // Configuración de los pines GPIO como salidas
//     esp_err_t res = gpio_set_direction(GPIO_XSHUT_FIRST, GPIO_MODE_OUTPUT);
//     if (res != ESP_OK) {
//         ESP_LOGE(TAG, "Error en el inicio del GPIO: direction XSHUT_FIRST");
//         return res;
//     }
//     res = gpio_set_level(GPIO_XSHUT_FIRST, 0);
//     if (res != ESP_OK) {
//         ESP_LOGE(TAG, "Error en el inicio del GPIO: level XSHUT_FIRST");
//         return res;
//     }

//     // res = gpio_set_direction(GPIO_XSHUT_SECOND, GPIO_MODE_OUTPUT);
//     // if (res != ESP_OK) {
//     //     ESP_LOGE(TAG, "Error en el inicio del GPIO: direction XSHUT_SECOND");
//     //     return res;
//     // }
//     // res = gpio_set_level(GPIO_XSHUT_SECOND, 0);
//     // if (res != ESP_OK) {
//     //     ESP_LOGE(TAG, "Error en el inicio del GPIO: level XSHUT_SECOND");
//     //     return res;
//     // }

//     // res = gpio_set_direction(GPIO_XSHUT_THIRD, GPIO_MODE_OUTPUT);
//     // if (res != ESP_OK) {
//     //     ESP_LOGE(TAG, "Error en el inicio del GPIO: direction XSHUT_THIRD");
//     //     return res;
//     // }
//     // res = gpio_set_level(GPIO_XSHUT_THIRD, 0);
//     // if (res != ESP_OK) {
//     //     ESP_LOGE(TAG, "Error en el inicio del GPIO: level XSHUT_THIRD");
//     //     return res;
//     // }

//     return res;
// }

// esp_err_t gpio_set_output(gpio_num_t gpio, bool enable)
// {
//     esp_err_t res = ESP_FAIL;
//     if (enable) {
//         res = gpio_set_level(gpio, 1); // Establecer el nivel alto
//     } else {
//         res = gpio_set_level(gpio, 0); // Establecer el nivel bajo
//     }
//     return res;
// }

#include "gpio.h"
#include "esp_err.h"

static const char *TAG = "GPIO";

esp_err_t gpio_init(void)
{
    gpio_config_t io_conf = {
        //.pin_bit_mask = (1ULL << GPIO_XSHUT_FIRST) | (1ULL << GPIO_XSHUT_SECOND) | (1ULL << GPIO_XSHUT_THIRD),
        .pin_bit_mask = (1ULL << GPIO_XSHUT_FIRST),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    if (gpio_config(&io_conf) != ESP_OK) {
        ESP_LOGE(TAG, "Error en el inicio del GPIO: direction XSHUT_FIRST");
        return ESP_FAIL;
    }

    // Asegurar que los pines inicien en nivel bajo
    //if (gpio_set_level(GPIO_XSHUT_FIRST, 0) != ESP_OK || gpio_set_level(GPIO_XSHUT_SECOND, 0) != ESP_OK || gpio_set_level(GPIO_XSHUT_THIRD, 0) != ESP_OK) {
    if (gpio_set_level(GPIO_XSHUT_FIRST, 0) != ESP_OK ) {
        ESP_LOGE(TAG, "Error en el inicio del GPIO: level XSHUT_FIRST");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t gpio_set_output(gpio_t gpio, bool enable)
{
    if (gpio_set_level(gpio, enable ? 1 : 0) == ESP_OK) {
        return ESP_OK;
    }
    ESP_LOGE(TAG, "Error en el seteo del GPIO: fallo al setear el nivel de XSHUT_FIRST");
    return ESP_FAIL;
}
