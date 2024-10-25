#include "servomotor.h"
#include "driver/mcpwm_prelude.h"  // Nuevo controlador MCPWM
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "SERVOMOTOR";

static mcpwm_cmpr_handle_t comparator = NULL;

static inline uint32_t angle_to_compare(uint16_t angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

esp_err_t servomotor_init()
{
    ESP_LOGI(TAG, "Create timer and operator");
    mcpwm_timer_handle_t timer = NULL;

    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP};

    esp_err_t err = mcpwm_new_timer(&timer_config, &timer);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initialaizing Servomotor - Timer: %s", esp_err_to_name(err));
        return err;
    }

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0};

    esp_err_t err2 = mcpwm_new_operator(&operator_config, &oper);

    if (err2 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initialaizing Servomotor - Operator: %s", esp_err_to_name(err2));
        return err2;
    }

    esp_err_t err3 = mcpwm_operator_connect_timer(oper, timer);

    if (err3 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error conecting Servomotor Timer and Operator: %s", esp_err_to_name(err3));
        return err3;
    }

    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true};

    esp_err_t err4 = mcpwm_new_comparator(oper, &comparator_config, &comparator);

    if (err4 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initialaizing Servomotor Comparator: %s", esp_err_to_name(err4));
        return err4;
    }

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = SERVO_PULSE_GPIO};

    esp_err_t err5 = mcpwm_new_generator(oper, &generator_config, &generator);

    if (err5 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initialaizing Servomotor Generator: %s", esp_err_to_name(err5));
        return err5;
    }

    esp_err_t err6 = mcpwm_comparator_set_compare_value(comparator, angle_to_compare(0));

    if (err6 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting initial Servomotor comparing value and spinnign to start position: %s", esp_err_to_name(err6));
        return err6;
    }

    esp_err_t err7 = mcpwm_generator_set_action_on_timer_event(generator,
                                                               MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));

    if (err7 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting Servomotor generator actions: %s", esp_err_to_name(err7));
        return err7;
    }

    esp_err_t err8 = mcpwm_generator_set_action_on_compare_event(generator,
                                                                 MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW));

    if (err8 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting Servomotor generator actions: %s", esp_err_to_name(err8));
        return err8;
    }

    esp_err_t err9 = mcpwm_timer_enable(timer);

    if (err9 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error enabling servomotor timer: %s", esp_err_to_name(err9));
        return err9;
    }

    esp_err_t err10 = mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP);

    if (err10 != ESP_OK)
    {
        ESP_LOGE(TAG, "Error staring Servomotor timer: %s", esp_err_to_name(err10));
        return err10;
    }

    return ESP_OK;
}

esp_err_t servomotor_set_angle(uint16_t angle){
    
    if(angle < SERVO_MIN_DEGREE || angle > SERVO_MAX_DEGREE){
        ESP_LOGE(TAG,"Invalid angle: %d. Angle must bet between %d and %d degrees.",angle, SERVO_MIN_DEGREE, SERVO_MAX_DEGREE);
        return ESP_ERR_INVALID_ARG;
    }

    uint32_t compare_value = angle_to_compare(angle);

    if(comparator == NULL) {
        ESP_LOGE(TAG,"ERROR setting angle, Comparator is NULL!");
        return ESP_FAIL;
    }

    esp_err_t err = mcpwm_comparator_set_compare_value(comparator, compare_value);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error setting Servomotor comparator value: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Servomotor moved to angle: %d degrees", angle);

    return ESP_OK;
}


