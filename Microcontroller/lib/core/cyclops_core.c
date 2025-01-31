#include "cyclops_core.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "limit_switch.h"
#include "esp_log.h"
#include "motors.h"
#include "battery.h"
#include "mqtt_handler.h"
#include "ap_server.h"
#include "lights.h"
#include "mqtt_server.h"
#include "mapping.h"

static const char *TAG = "CYCLOPS_CORE";
TaskHandle_t servoInterruptionTaskHandler = NULL;
TaskHandle_t instructionHandlerTaskHandler = NULL;
TaskHandle_t receiveInstructionTaskHandler = NULL;
TaskHandle_t batteryTaskHandler = NULL;
TaskHandle_t mappingTaskHandler = NULL;

static void servoInterruptionTask(void *);
static void receiveInstruction(void *);
static void instructionHandler(void *);
static void executeInstruction(char *);
static void mappingTask(void *);
static void batteryTask(void *parameter);

esp_err_t system_init()
{
    esp_err_t err = ESP_OK;

    ESP_LOGI(TAG, "Iniciando Server Service...");
    err = initialize_server();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR SETTING UP SERVER:  %s", esp_err_to_name(err));
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Server Service Iniciado!");

    ESP_LOGI(TAG, "Esperando conexión del cliente...");
    err = wait_for_client_connection();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR - WAITING FOR CLIENT: %s", esp_err_to_name(err));
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Cliente Conectado!");

    ESP_LOGI(TAG, "Iniciando MQTT Service...");
    err = mqtt_start();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR SETTING UP MQTT SERVER: %s", esp_err_to_name(err));
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "MQTT Service Iniciado!");

    // ENVIO BIENVENIDA
    //err = sendBarrier();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al enviar barrera");
        return 1;
    }

    ESP_LOGI(TAG, "Iniciando Luces Service...");
    err = lights_init();
    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "Error Setting Up Lights: %s", esp_err_to_name(err));
    }
    ESP_LOGI(TAG, "Luces Service Iniciado!");

    ESP_LOGI(TAG, "Iniciando Motores...");
    motors_setup();
    ESP_LOGI(TAG, "Motores Iniciados!");

    ESP_LOGI(TAG, "Iniciando Mapping Service...");
    err = mapping_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR SETTING UP MAPPING");
    }
    ESP_LOGI(TAG, "Mapping Service Iniciado!");

    ESP_LOGI(TAG, "Iniciando Battery Service...");
    err = battery_sensor_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "ERROR SETTING UP BATTERY SENSOR");
    }
    ESP_LOGI(TAG, "Battery Service Iniciado!");

    return err;
}

esp_err_t createTasks()
{
    // BACKGROUND TASKs

    BaseType_t task_created = xTaskCreatePinnedToCore(
        servoInterruptionTask,         // Función de la tarea
        "ServoInterruptionTask",       // Nombre de la tarea
        4096,                          // Tamaño de la pila
        NULL,                          // Argumento de la tarea (NULL si no es necesario)
        1,                             // Prioridad
        &servoInterruptionTaskHandler, // Puntero al handle de la tarea (NULL si no es necesario)
        tskNO_AFFINITY                 // Núcleo (sin afinidad específica)
    );

    if (task_created != pdPASS)
    {
        ESP_LOGE(TAG, "Error Creating Limit Switch cheking Task");
        return ESP_FAIL; // Retorna error si la tarea no se pudo crear
    }

    // MAIN TASKs
    task_created = xTaskCreatePinnedToCore(
        instructionHandler,
        "InstructionsHadlerTask",
        4096,
        NULL,
        2,
        &instructionHandlerTaskHandler,
        tskNO_AFFINITY);

    if (task_created != pdPASS)
    {
        ESP_LOGE(TAG, "Error Creating Instruction HandlerTask");
        return ESP_FAIL; // Retorna error si la tarea no se pudo crear
    }

    task_created = xTaskCreatePinnedToCore(
        receiveInstruction,
        "receiveInstructionTask",
        4096,
        NULL,
        2,
        &receiveInstructionTaskHandler,
        tskNO_AFFINITY);

    if (task_created != pdPASS)
    {
        ESP_LOGE(TAG, "Error Creating  Receive Instruction Task");
        return ESP_FAIL;
    }

    task_created = xTaskCreatePinnedToCore(
        mappingTask,
        "MappingTask",
        4096,
        NULL,
        2,
        &mappingTaskHandler,
        tskNO_AFFINITY);

    if (task_created != pdPASS)
    {
        ESP_LOGE(TAG, "Error Creating Mapping Task");
        return ESP_FAIL; // Retorna error si la tarea no se pudo crear
    }

    task_created = xTaskCreatePinnedToCore(
        batteryTask,
        "BateryTask",
        4096,
        NULL,
        2,
        &batteryTaskHandler,
        tskNO_AFFINITY);

    if (task_created != pdPASS)
    {
        ESP_LOGE(TAG, "Error Creating Battery Task");
        return ESP_FAIL; // Retorna error si la tarea no se pudo crear
    }
    return ESP_OK; // Retorna éxito si la tarea fue creada correctamente
}

void abort_tasks()
{
    if (servoInterruptionTaskHandler != NULL)
    {
        vTaskDelete(servoInterruptionTaskHandler);
        servoInterruptionTaskHandler = NULL;
    }
    if (instructionHandlerTaskHandler != NULL)
    {
        vTaskDelete(instructionHandlerTaskHandler);
        instructionHandlerTaskHandler = NULL;
    }
    if (receiveInstructionTaskHandler != NULL)
    {
        vTaskDelete(receiveInstructionTaskHandler);
        receiveInstructionTaskHandler = NULL;
    }
    if (batteryTaskHandler != NULL)
    {
        vTaskDelete(batteryTaskHandler);
        batteryTaskHandler = NULL;
    }
    if (mappingTaskHandler != NULL)
    {
        vTaskDelete(mappingTaskHandler);
        mappingTaskHandler = NULL;
    }

    // REST OF TASKS
}

// TASK BODY
static void servoInterruptionTask(void *parameter)
{
    while (1)
    {
        check_limit_switch();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void receiveInstruction(void *parameter)
{
    static char inst[20];
    esp_err_t err = ESP_OK;
    while (1)
    {
        err = getHTTPInstruction(inst, sizeof(inst));
        if (err == ESP_OK)
        {
            if (saveInstruction(inst) != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to save instruction");
            }
            ESP_LOGW(TAG, "INST RECEIVED - %s", inst);
        }
        else if (err == ESP_ERR_TIMEOUT)
        {
            ESP_LOGE(TAG, "ERROR GETTING INSTRUCTION");
        }
        memset(inst, 0, sizeof(inst));
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void instructionHandler(void *parameter)
{
    char inst[20];
    esp_err_t err = ESP_OK;
    while (1)
    {
        err = getInstruccionMessage(inst);
        if (err == ESP_OK)
        {
            executeInstruction(inst);
            ESP_LOGW(TAG, "INST Handled - %s", inst);
        }
        else if (err == ESP_ERR_TIMEOUT)
        {
            ESP_LOGE(TAG, "ERROR GETTING INSTRUCTION");
        }
        memset(inst, 0, 20);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

static void executeInstruction(char *inst)
{
    if (strncmp(inst, "Brake", 5) == 0)
    {
        motors_command(STOP);
    }
    else if (strncmp(inst, "Backward", 8) == 0)
    {
        motors_command(BACKWARD);
    }
    else if (strncmp(inst, "Forward", 7) == 0)
    {
        motors_command(FORWARD);
    }
    else if (strncmp(inst, "Right", 5) == 0)
    {
        motors_command(ROTATE_RIGHT);
    }
    else if (strncmp(inst, "Left", 4) == 0)
    {
        motors_command(ROTATE_LEFT);
    }
    else if (strncmp(inst, "SpeedUp", 7) == 0)
    {
        servo_set_speed(UP);
    }
    else if (strncmp(inst, "SpeedDown", 9) == 0)
    {
        servo_set_speed(DOWN);
    }
    else if (strncmp(inst, "Pause", 5) == 0)
    {

        if (mapping_pause() != ESP_OK)
        {
            ESP_LOGE(TAG, "ERROR TRYING TO STOP SERVO");
        }
        else
        {
            vTaskSuspend(mappingTaskHandler);
            ESP_LOGE(TAG, "Mapping task suspended");
        }
    }
    else if (strncmp(inst, "Play", 4) == 0)
    {
        if (mapping_restart() != ESP_OK)
        {
            ESP_LOGE(TAG, "ERROR TRYING TO RESTART SERVO");
        }
        else
        {
            vTaskResume(mappingTaskHandler);
            ESP_LOGE(TAG, "Mapping task resumed");
        }
    }
}

static void batteryTask(void *parameter)
{
    esp_err_t err = ESP_OK;
    uint8_t level = 0;
    while (1)
    {
        err = battery_sensor_read(&level);
        if (err != ESP_OK)
        {

            ESP_LOGE(TAG, "Error Reading Battery Level %s", esp_err_to_name(err));
        }
        else
        {
            ESP_LOGW(TAG, "Battery Level: %u", level);
            if(sendBatteryLevel(level) != ESP_OK){
                ESP_LOGE(TAG,"ERROR SENDING BATTERY LEVEL");
            }
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

static void mappingTask(void *parameter)
{
    uint16_t distance = 0;
    uint16_t angle = 0;
    esp_err_t err = ESP_OK;
    while (1)
    {
        err = getMappingValue(&angle, &distance);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "FAIL TO GET MAPPING VALUE");
        }
        {
            ESP_LOGW(TAG, "Dist: %u - Ang: %u", distance, angle);
            sendMappingValue(distance,angle);
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}