#include "servo_interruptions.h"
#include "esp_log.h"
#include "servo.h"
#include "esp_timer.h" // Para obtener el tiempo actual en microsegundos
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

const char *TAG = "Servo Interruptions";
static volatile bool limit_switch_triggered = false; // Variable para el estado de la interrupción
static int64_t last_trigger_time = 0;                // Tiempo de la última activación en microsegundos
const int64_t debounce_delay = 500000;               // Retardo de debounce en microsegundos (50 ms)

static SemaphoreHandle_t flag_semaphore;

static void limit_switch_isr_handler(void *arg)
{
    int64_t current_time = esp_timer_get_time();
    // Comprobar si ha pasado el tiempo de debounce
    if ((current_time - last_trigger_time) > debounce_delay)
    {
        limit_switch_triggered = true;
        last_trigger_time = current_time; // Actualiza el tiempo de la última activación
        xSemaphoreGiveFromISR(flag_semaphore, NULL);
    }
}

esp_err_t interrupt_init()
{

    flag_semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(flag_semaphore);

    esp_err_t err = gpio_set_direction(LIMIT_SWITCH_PIN, GPIO_MODE_INPUT);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO error. Failed to set direction.");
        return err;
    }

    err = gpio_set_intr_type(LIMIT_SWITCH_PIN, GPIO_INTR_NEGEDGE); // cuando pasa de alto a bajo
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Parameter error. Failed to set interrupt trigger type.");
        return err;
    }

    // Habilita la resistencia pull-up para asegurar un estado alto cuando no está activado
    err = gpio_pullup_en(LIMIT_SWITCH_PIN);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Parameter error. Failed to enable pull-up on GPIO.");
        return err;
    }

    err = gpio_install_isr_service(0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to install the GPIO driver's ISR handler service.");
        return err;
    }

    err = gpio_isr_handler_add(LIMIT_SWITCH_PIN, limit_switch_isr_handler, NULL);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add ISR handler for the corresponding GPIO pin.");
        return err;
    }

    return ESP_OK;
}

void check_limit_switch()
{

    if (xSemaphoreTake(flag_semaphore, portMAX_DELAY) == pdTRUE)
    {
        if (limit_switch_triggered)
        {
            ESP_LOGW(TAG, "Limit switch triggered! Servo has reached the target position.");
            servo_invert();
            limit_switch_triggered = false; // Restablece el estado de la interrupción
            ESP_LOGW(TAG, "#");
        }
        xSemaphoreGive(flag_semaphore);
    }
}
