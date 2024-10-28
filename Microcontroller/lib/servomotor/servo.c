#include "servo.h"
#include "driver/mcpwm_prelude.h"
#include "esp_log.h"

// Please consult the datasheet of your servo before changing the following parameters
#define SERVO_MIN_PULSEWIDTH_US 900  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2100 // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE 0        // Minimum angle
#define SERVO_MAX_DEGREE 180          // Maximum angle

#define SERVO_PULSE_GPIO 16                  // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000 // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD 20000          // 20000 ticks, 20ms

static const char *TAG = "SERVOMOTOR";
static mcpwm_cmpr_handle_t comparator = NULL;

// Functions prototypes
static inline uint32_t angle_to_compare(int);

esp_err_t servo_initialize(void)
{

    ESP_LOGI(TAG, "Creating timer and operator...");
    mcpwm_timer_handle_t timer = NULL;
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

    ESP_LOGI(TAG, "All set and initialized");
    ESP_LOGI(TAG, "Setting Servo to start position (0°)...");
    // set the initial compare value, so that the servo will spin to the center position
    if (mcpwm_comparator_set_compare_value(comparator, angle_to_compare(0)) != ESP_OK)
    {
        ESP_LOGW(TAG, "WARNING Setting Servo to start position FAIL");
    }

    ESP_LOGI(TAG, "Set Generator action on Timer and Compare event");
    // go high on counter empty
    if (mcpwm_generator_set_action_on_timer_event(generator, MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Setting Generator Timer Event");
        return ESP_FAIL;
    }
    // go low on compare threshold
    if (mcpwm_generator_set_action_on_compare_event(generator, MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Setting Generator Comparator Event");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Enabling and start timer...");
    if (mcpwm_timer_enable(timer) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Enabling Timer");
        return ESP_FAIL;
    }
    if (mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Starting Timer");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t servo_write_angle(int angle)
{
    if (mcpwm_comparator_set_compare_value(comparator, angle_to_compare(angle)) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Setting angle");
        return ESP_FAIL;
    }

    return ESP_OK;
}

static inline uint32_t angle_to_compare(int angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}