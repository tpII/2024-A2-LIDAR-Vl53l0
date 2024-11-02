#include "servo.h"
#include "driver/mcpwm_prelude.h"
#include "esp_log.h"

// Parámetros específicos para el servomotor de giro continuo
#define SERVO_MIN_PULSEWIDTH_US 900   // Ancho de pulso mínimo (giro rápido en un sentido)
#define SERVO_MAX_PULSEWIDTH_US 2100  // Ancho de pulso máximo (giro rápido en el sentido opuesto)
#define SERVO_STOP_PULSEWIDTH_US 1500 // Ancho de pulso para detener el servo

#define SERVO_PULSE_GPIO 4                  // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000 // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD 20000          // 20000 ticks, 20ms

static const char *TAG = "SERVOMOTOR";
static mcpwm_cmpr_handle_t comparator = NULL;
static mcpwm_timer_handle_t timer = NULL;

esp_err_t servo_initialize(void)
{

    ESP_LOGI(TAG, "Creating timer and operator...");
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };

    if (mcpwm_new_timer(&timer_config, &timer) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Creating Timer");
        return ESP_FAIL;
    }

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };

    if (mcpwm_new_operator(&operator_config, &oper) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Creating Operator");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Connecting timer and operator...");
    if (mcpwm_operator_connect_timer(oper, timer) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Connecting Timer and Operator");
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI(TAG, "Timer and Operator connected succesfully");
    }

    ESP_LOGI(TAG, "Creating comparator and generator from the operator");
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };

    if (mcpwm_new_comparator(oper, &comparator_config, &comparator) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Creating Comparator");
        return ESP_FAIL;
    }

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = SERVO_PULSE_GPIO,
    };

    if (mcpwm_new_generator(oper, &generator_config, &generator) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Creating Generator");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Set Generator action on Timer and Compare event");

    if (mcpwm_generator_set_action_on_timer_event(generator, MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)) != ESP_OK ||
        mcpwm_generator_set_action_on_compare_event(generator, MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al configurar los eventos de generador");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Enabling and start timer...");
    if (mcpwm_timer_enable(timer) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Enabling Timer");
        return ESP_FAIL;
    }

    if (mcpwm_timer_start_stop(timer,MCPWM_TIMER_START_NO_STOP) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Starting Timer");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Servomotor inicializado correctamente");
    return ESP_OK;
}

esp_err_t servo_start(void)
{
    return servo_set_speed(SERVO_MIN_PULSEWIDTH_US); // Inicia con el duty en 900us
}

esp_err_t servo_stop(void)
{
    return servo_set_speed(SERVO_STOP); // Detiene el servo con el duty en 1500us
}

// Función para ajustar la velocidad del servomotor
esp_err_t servo_set_speed(uint32_t duty)
{
    if (duty < SERVO_MIN_PULSEWIDTH_US || duty > SERVO_MAX_PULSEWIDTH_US)
    {
        ESP_LOGE(TAG, "Duty fuera del rango permitido");
        return ESP_ERR_INVALID_ARG;
    }
    ESP_LOGW(TAG, "DUTY IN RANGE, %lu", duty);

    if (mcpwm_comparator_set_compare_value(comparator, duty) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al ajustar la velocidad");
        return ESP_FAIL;
    }

    return ESP_OK;
}