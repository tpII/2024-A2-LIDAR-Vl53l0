#include "gpio.h"

#define GPIO_XSHUT_FIRST GPIO_NUM_23 // Cambia esto al número de pin que desees
// #define GPIO_XSHUT_SECOND GPIO_NUM_NC // Cambia esto al número de pin que desees
// #define GPIO_XSHUT_THIRD GPIO_NUM_NC // Cambia esto al número de pin que desees

static const char *TAG = "GPIO";

esp_err_t gpio_init(void)
{
    // Configuración de los pines GPIO como salidas
    esp_err_t res = gpio_set_direction(GPIO_XSHUT_FIRST, GPIO_MODE_OUTPUT);
    if (res != ESP_OK) {
        ESP_LOGE(TAG, "Error en el inicio del GPIO: direction XSHUT_FIRST");
        return res;
    }
    res = gpio_set_level(GPIO_XSHUT_FIRST, 0);
    if (res != ESP_OK) {
        ESP_LOGE(TAG, "Error en el inicio del GPIO: level XSHUT_FIRST");
        return res;
    }

    // res = gpio_set_direction(GPIO_XSHUT_SECOND, GPIO_MODE_OUTPUT);
    // if (res != ESP_OK) {
    //     ESP_LOGE(TAG, "Error en el inicio del GPIO: direction XSHUT_SECOND");
    //     return res;
    // }
    // res = gpio_set_level(GPIO_XSHUT_SECOND, 0);
    // if (res != ESP_OK) {
    //     ESP_LOGE(TAG, "Error en el inicio del GPIO: level XSHUT_SECOND");
    //     return res;
    // }

    // res = gpio_set_direction(GPIO_XSHUT_THIRD, GPIO_MODE_OUTPUT);
    // if (res != ESP_OK) {
    //     ESP_LOGE(TAG, "Error en el inicio del GPIO: direction XSHUT_THIRD");
    //     return res;
    // }
    // res = gpio_set_level(GPIO_XSHUT_THIRD, 0);
    // if (res != ESP_OK) {
    //     ESP_LOGE(TAG, "Error en el inicio del GPIO: level XSHUT_THIRD");
    //     return res;
    // }

    return res;
}

esp_err_t gpio_set_output(gpio_num_t gpio, bool enable)
{
    esp_err_t res = ESP_FAIL;
    if (enable) {
        res = gpio_set_level(gpio, 1); // Establecer el nivel alto
    } else {
        res = gpio_set_level(gpio, 0); // Establecer el nivel bajo
    }
    return res;
}
