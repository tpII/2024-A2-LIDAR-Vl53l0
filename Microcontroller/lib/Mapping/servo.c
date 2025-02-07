#include "servo.h"
#include "driver/mcpwm_prelude.h"
#include "esp_log.h"
#include "limit_switch.h"
#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <string.h>
#include <driver/gpio.h>
#include "debug_helper.h"


// Parámetros específicos para el servomotor de giro continuo
#define SERVO_MIN_PULSEWIDTH_US 900   // Ancho de pulso mínimo (giro rápido en un sentido)
#define SERVO_MAX_PULSEWIDTH_US 2100  // Ancho de pulso máximo (giro rápido en el sentido opuesto)
#define SERVO_STOP_PULSEWIDTH_US 1500 // Ancho de pulso para detener el servo

#define SERVO_PULSE_GPIO GPIO_NUM_14         // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000 // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD 20000          // 20000 ticks, 20ms

#define CONVERSION_FACTOR 1000000
#define BASE_SPEED 545.45
#define DIFFERENTIAL 600
#define W_5V 0.00055  // grados/µs

// TIMER VARIABLES
static const char *TAG = "SERVOMOTOR";
static mcpwm_cmpr_handle_t comparator = NULL;
static mcpwm_timer_handle_t timer = NULL;
static uint32_t current_duty = SERVO_STOP_PULSEWIDTH_US;

// SERVO VARIABLES
static const int16_t cw_limit = 30;
static const int16_t ccw_limit = -30;
static volatile uint64_t time_base = 0;
static volatile uint16_t last_angle_offset = 0;
static volatile uint16_t angle = 0;
static volatile uint32_t next_speed = 0;
static volatile bool change_speed_flag = false;
static volatile uint64_t partialTimeBase = 0;
static volatile uint64_t pauseTimeBase = 0;

// static portMUX_TYPE limit_mux = portMUX_INITIALIZER_UNLOCKED;
static SemaphoreHandle_t limit_semaphore;

// static portMUX_TYPE current_duty_mux = portMUX_INITIALIZER_UNLOCKED;
static SemaphoreHandle_t current_duty_semaphore;

// static portMUX_TYPE speed_change_mux = portMUX_INITIALIZER_UNLOCKED;
static SemaphoreHandle_t speed_change_semaphore;

static esp_err_t servo_set_speed_ISR(uint32_t);

esp_err_t servo_initialize(void)
{

    limit_semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(limit_semaphore);
    if(limit_semaphore == NULL)
    {
        ESP_LOGE(TAG,"ERROR: limit_semaphore is NULL");
        return ESP_FAIL;
    }
    current_duty_semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(current_duty_semaphore);
    if(current_duty_semaphore == NULL)
    {
        ESP_LOGE(TAG,"ERROR: current_duty_semaphore is NULL");
        return ESP_FAIL;
    }
    speed_change_semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(speed_change_semaphore);
    if(speed_change_semaphore == NULL)
    {
        ESP_LOGE(TAG,"ERROR: speed_change_semaphore is NULL");
        return ESP_FAIL;
    }
    
    

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Creating timer and operator..."));
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

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Connecting timer and operator..."));
    if (mcpwm_operator_connect_timer(oper, timer) != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Connecting Timer and Operator");
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI(TAG, "Timer and Operator connected succesfully");
    }

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Creating comparator and generator from the operator"));
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

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Set Generator action on Timer and Compare event"));

    if (mcpwm_generator_set_action_on_timer_event(generator, MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)) != ESP_OK ||
        mcpwm_generator_set_action_on_compare_event(generator, MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al configurar los eventos de generador");
        return ESP_FAIL;
    }

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Enabling and start timer..."));
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

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Initializing Servo Interruption"));
    if (interrupt_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR Initializing Servo Interruption");
        return ESP_FAIL;
    }

    DEBUGING_ESP_LOG(ESP_LOGI(TAG, "Servomotor inicializado correctamente"));
    return ESP_OK;
}

esp_err_t servo_start(void)
{

    return servo_set_speed_ISR(SERVO_MEDIUM_SPEED_CCW); // Inicia con el duty en 900us
}

esp_err_t servo_stop(void)
{
    return servo_set_speed_ISR(SERVO_STOP); // Detiene el servo con el duty en 1500us
}

// Función para ajustar la velocidad del servomotor
static esp_err_t servo_set_speed_ISR(uint32_t duty)
{
    if (duty < SERVO_MIN_PULSEWIDTH_US || duty > SERVO_MAX_PULSEWIDTH_US)
    {
        ESP_LOGE(TAG, "Duty fuera del rango permitido");
        return ESP_ERR_INVALID_ARG;
    }
    DEBUGING_ESP_LOG(ESP_LOGW(TAG, "DUTY IN RANGE, %lu", duty));

    if (mcpwm_comparator_set_compare_value(comparator, duty) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al ajustar la velocidad");
        return ESP_FAIL;
    }
    // SE PUEDE QUITAR AL MODIFICAR LA RUTINA DE INICIO.
    if (xSemaphoreTakeFromISR(current_duty_semaphore, portMAX_DELAY) == pdTRUE)
    {
        current_duty = duty;
        xSemaphoreGive(current_duty_semaphore);
    }
    return ESP_OK;
}

void servo_invert()
{
    esp_err_t err = ESP_OK;

    if (xSemaphoreTake(current_duty_semaphore, portMAX_DELAY) == pdTRUE)
    {
        if (xSemaphoreTake(speed_change_semaphore, portMAX_DELAY) == pdTRUE)
        {
            if (change_speed_flag)
            {
                change_speed_flag = false;
                current_duty = next_speed;
            }
            xSemaphoreGive(speed_change_semaphore);
        }
        if (current_duty == SERVO_STOP)
        {
            ESP_LOGW(TAG, "Error: Trying to invert orientation while servo is stopped");
        }
        else if (current_duty > SERVO_STOP)
        {
            if (xSemaphoreTake(limit_semaphore, portMAX_DELAY) == pdTRUE)
            {
                last_angle_offset = ccw_limit;
                time_base = esp_timer_get_time();
                xSemaphoreGive(limit_semaphore);
            }
            switch (current_duty)
            {
            case SERVO_LOW_SPEED_CCW:
                xSemaphoreGive(current_duty_semaphore);
                err = servo_set_speed_ISR(SERVO_LOW_SPEED_CW);
                break;
            case SERVO_MEDIUM_SPEED_CCW:
                xSemaphoreGive(current_duty_semaphore);
                err = servo_set_speed_ISR(SERVO_MEDIUM_SPEED_CW);
                break;
            case SERVO_MAX_SPEED_CCW:
                xSemaphoreGive(current_duty_semaphore);
                err = servo_set_speed_ISR(SERVO_MAX_SPEED_CW);
                break;
            default:
                xSemaphoreGive(current_duty_semaphore);
                break;
            }
        }
        else if (current_duty < SERVO_STOP)
        {
            if (xSemaphoreTake(limit_semaphore, portMAX_DELAY) == pdTRUE)
            {
                last_angle_offset = cw_limit;
                time_base = esp_timer_get_time();
                xSemaphoreGive(limit_semaphore);
            }
            switch (current_duty)
            {
            case SERVO_LOW_SPEED_CW:
                xSemaphoreGive(current_duty_semaphore);
                err = servo_set_speed_ISR(SERVO_LOW_SPEED_CCW);
                break;
            case SERVO_MEDIUM_SPEED_CW:
                xSemaphoreGive(current_duty_semaphore);
                err = servo_set_speed_ISR(SERVO_MEDIUM_SPEED_CCW);
                break;
            case SERVO_MAX_SPEED_CW:
                xSemaphoreGive(current_duty_semaphore);
                err = servo_set_speed_ISR(SERVO_MAX_SPEED_CCW);
                break;
            default:
                xSemaphoreGive(current_duty_semaphore);
                break;
            }
        }
    }
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error: Trying to invert servo orientation, rebooting servo....");
    }
    DEBUGING_ESP_LOG(ESP_LOGW(TAG, "INVERT END"));
}

int16_t readAngle()
{
    uint16_t duty = 0;
    uint64_t time_now = esp_timer_get_time();
    int16_t angle_offset = 0;
    uint64_t time_reference = 0;

    // Obtener variables necesarias con semáforo
    if (xSemaphoreTake(limit_semaphore, portMAX_DELAY) == pdTRUE)
    {
        angle_offset = last_angle_offset;
        time_reference = time_base;
        xSemaphoreGive(limit_semaphore);
    }

    if(time_reference == 0){
        return -1;
    }

    if (xSemaphoreTake(current_duty_semaphore, portMAX_DELAY) == pdTRUE)
    {
        duty = current_duty;
        xSemaphoreGive(current_duty_semaphore);
    }

    // Calcular velocidad escalada y ángulo
    int64_t temp = ((int64_t)BASE_SPEED * (duty - SERVO_STOP)) * (time_now - time_reference);
    int16_t angle = (int16_t)(((temp / (DIFFERENTIAL * CONVERSION_FACTOR)) + angle_offset) % 360);
    ESP_LOGE(TAG, "Angle = %" PRIi16, angle);

    DEBUGING_ESP_LOG(ESP_LOGW(TAG, "Angle = %" PRIi16, angle));

    return (angle);
}

void servo_set_speed(SERVO_DIRECTION dir)
{
    static volatile uint32_t duty = 0;
    if (xSemaphoreTake(current_duty_semaphore, portMAX_DELAY) == pdTRUE)
    {
        if (current_duty != SERVO_STOP)
        {
            switch (dir)
            {

            case UP:
                if (current_duty == SERVO_LOW_SPEED_CCW || current_duty == SERVO_LOW_SPEED_CW)
                {
                    if (current_duty == SERVO_LOW_SPEED_CCW)
                    {
                        duty = SERVO_MEDIUM_SPEED_CCW;
                    }
                    else
                    {
                        duty = SERVO_MEDIUM_SPEED_CW;
                    }
                }
                else
                {
                    if (current_duty == SERVO_MEDIUM_SPEED_CCW || current_duty == SERVO_MEDIUM_SPEED_CW)
                    {
                        if (current_duty == SERVO_MEDIUM_SPEED_CCW)
                        {
                            duty = SERVO_MAX_SPEED_CCW;
                        }
                        else
                        {
                            duty = SERVO_MAX_SPEED_CW;
                        }
                    }
                }
                break;
            case DOWN:
                if (current_duty == SERVO_MAX_SPEED_CCW || current_duty == SERVO_MAX_SPEED_CW)
                {
                    if (current_duty == SERVO_MAX_SPEED_CCW)
                    {
                        duty = SERVO_MEDIUM_SPEED_CCW;
                    }
                    else
                    {
                        duty = SERVO_MEDIUM_SPEED_CW;
                    }
                }
                else
                {
                    if (current_duty == SERVO_MEDIUM_SPEED_CCW || current_duty == SERVO_MEDIUM_SPEED_CW)
                    {
                        if (current_duty == SERVO_MEDIUM_SPEED_CCW)
                        {
                            duty = SERVO_LOW_SPEED_CCW;
                        }
                        else
                        {
                            duty = SERVO_LOW_SPEED_CW;
                        }
                    }
                }

                break;
            default:
                current_duty = SERVO_STOP;
                break;
            }
        }
        DEBUGING_ESP_LOG(ESP_LOGE(TAG,"VELOCIDAD ACTUAL: %lu",current_duty));
        xSemaphoreGive(current_duty_semaphore);
    }
    if (xSemaphoreTake(speed_change_semaphore, portMAX_DELAY) == pdTRUE)
    {
        next_speed = duty;
        change_speed_flag = 1;
        xSemaphoreGive(speed_change_semaphore);
    }
}

esp_err_t servo_pause(){

    if(xSemaphoreTake(limit_semaphore,portMAX_DELAY) == pdTRUE){
        if(servo_stop() != ESP_OK){
            ESP_LOGE(TAG,"FAIL TO PAUSE SERVO");
            xSemaphoreGive(limit_semaphore);
            return ESP_FAIL;
        }
        partialTimeBase = time_base;
        pauseTimeBase = esp_timer_get_time();
        xSemaphoreGive(limit_semaphore);
        return ESP_OK;
    }
    return ESP_ERR_TIMEOUT;
}

esp_err_t servo_restart(){
    uint64_t auxTime=0;
    if(xSemaphoreTake(limit_semaphore,portMAX_DELAY) == pdTRUE){
        auxTime = esp_timer_get_time() - pauseTimeBase;
        if(servo_start() != ESP_OK){
            ESP_LOGE(TAG,"FAIL TO RESTART SERVO");
            xSemaphoreGive(limit_semaphore);
            return ESP_FAIL;
        }
        partialTimeBase -= auxTime;
        xSemaphoreGive(limit_semaphore);
    }
    else 
    {
        return ESP_ERR_TIMEOUT;
    }
    return ESP_OK;
}

esp_err_t delete_servo_semaphores()
{
    if (limit_semaphore != NULL) {
        vSemaphoreDelete(limit_semaphore);
    }
    else {
        return ESP_FAIL;
    }

    if (current_duty_semaphore != NULL) {
        vSemaphoreDelete(current_duty_semaphore);
    }
    else {
        return ESP_FAIL;
    }

    if (speed_change_semaphore != NULL) {
        vSemaphoreDelete(speed_change_semaphore);
    }
    else {
        return ESP_FAIL;
    }

    return ESP_OK;
}