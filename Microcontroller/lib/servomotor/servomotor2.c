#include "servomotor2.h"
#include "driver/ledc.h"
#include "esp_log.h"

#define SERVO_PIN GPIO_NUM_16
#define MIN_PULSE_WIDTH_US 600
#define MAX_PULSE_WIDTH_US 2100
#define MAX_ANGLE 180.0
#define PWM_FREQ_HZ 50
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_RESOLUTION LEDC_TIMER_16_BIT

static const char *TAG = "SERVOMOTOR";
static uint32_t full_duty = 0;


static uint32_t calculate_duty(float angle) {
    float pulse_width = (angle / MAX_ANGLE) * (MAX_PULSE_WIDTH_US - MIN_PULSE_WIDTH_US) + MIN_PULSE_WIDTH_US;
    return (uint32_t)((pulse_width * full_duty * PWM_FREQ_HZ) / 1000000.0f);
}

static float calculate_angle(uint32_t duty) {
    float pulse_width = (float)duty * 1000000.0f / full_duty / PWM_FREQ_HZ;
    float angle = ((pulse_width - MIN_PULSE_WIDTH_US) * MAX_ANGLE) / (MAX_PULSE_WIDTH_US - MIN_PULSE_WIDTH_US);
    return angle;
}
esp_err_t initialize_servo() {
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_RESOLUTION,
        .freq_hz = PWM_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    
    if (ledc_timer_config(&timer_config) != ESP_OK) {
        ESP_LOGE(TAG, "LEDC timer configuration failed");
        return ESP_FAIL;
    }
    
    ledc_channel_config_t channel_config = {
        .gpio_num = SERVO_PIN,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    
    if (ledc_channel_config(&channel_config) != ESP_OK) {
        ESP_LOGE(TAG, "LEDC channel configuration failed");
        return ESP_FAIL;
    }

    full_duty = (1 << LEDC_RESOLUTION) - 1;
    
    return ESP_OK;
}

esp_err_t write_angle(float angle) {
    if (angle < 0.0 || angle > MAX_ANGLE) {
        ESP_LOGE(TAG, "Invalid angle: %f", angle);
        return ESP_ERR_INVALID_ARG;
    }

    uint32_t duty = calculate_duty(angle);
    esp_err_t ret = ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
    ret |= ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set duty cycle");
        return ESP_FAIL;
    }

    return ESP_OK;
}


esp_err_t read_angle(float *angle) {
    if (angle == NULL) {
        ESP_LOGE(TAG, "Invalid pointer for angle");
        return ESP_ERR_INVALID_ARG;
    }

    uint32_t duty = ledc_get_duty(LEDC_MODE, LEDC_CHANNEL);
    *angle = calculate_angle(duty);

    return ESP_OK;
}