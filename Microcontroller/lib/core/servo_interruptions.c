#include "servo_interruptions.h"

static void limit_switch_isr_handler(void* arg);

const char *TAG = "Servo Interruptions";

static void limit_switch_isr_handler(void* arg) {
    // modificar (no se puede usar printf o ESP_LOGE)
    printf("Limit switch triggered! Servo has reached the target position.\n");
}

esp_err_t interrupt_init(void) {
    esp_err_t err = gpio_set_direction(LIMIT_SWITCH_PIN, GPIO_MODE_INPUT);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "GPIO error. Failed to set direction.");
        return err;
    }

    err = gpio_set_intr_type(LIMIT_SWITCH_PIN, GPIO_INTR_NEGEDGE); // cuando pasa de alto a bajo
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Parameter error. Failed to set interrupt trigger type.");
        return err;
    }

    // Habilita la resistencia pull-up para asegurar un estado alto cuando no está activado
    err = gpio_pullup_en(LIMIT_SWITCH_PIN); 
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Parameter error. Failed to enable pull-up on GPIO.");
        return err;
    }

    err = gpio_install_isr_service(0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install the GPIO driver's ISR handler service.");
        return err;
    }

    err = gpio_isr_handler_add(LIMIT_SWITCH_PIN, limit_switch_isr_handler, NULL);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add ISR handler for the corresponding GPIO pin.");
        return err;
    }
    return ESP_OK;
}
