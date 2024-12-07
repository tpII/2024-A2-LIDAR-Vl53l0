#include "freeRtosTasks.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "limit_switch.h"
#include "instruction_buffer.h"
#include "esp_log.h"
#include "motors.h"
#include "servo.h"

const char *TAG = "TASKS";
static void servoInterruptionTask(void *);

TaskHandle_t servoInterruptionTaskHandler = NULL;
TaskHandle_t instructionHandlerTaskHandler = NULL;
esp_err_t createTasks()
{
    // BACKGROUND TASKs
    BaseType_t task_created = xTaskCreatePinnedToCore(
        servoInterruptionTask,         // Función de la tarea
        "ServoInterruptionTask",       // Nombre de la tarea
        2048,                          // Tamaño de la pila
        NULL,                          // Argumento de la tarea (NULL si no es necesario)
        1,                             // Prioridad
        &servoInterruptionTaskHandler, // Puntero al handle de la tarea (NULL si no es necesario)
        tskNO_AFFINITY                 // Núcleo (sin afinidad específica)
    );

    if (task_created != pdPASS)
    {
        return ESP_FAIL; // Retorna error si la tarea no se pudo crear
    }

    BaseType_t task_created = xTaskCreatePinnedToCore(
        instructionHandler,
        2048,
        NULL,
        2,
        &instructionHandlerTaskHandler,
        tskNo_AFFINITY);
    // MAIN TASKs

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

static void instructionHandler(void *parameter)
{
    char *inst;
    esp_err_t err = ESP_OK;
    while (1)
    {
        err = getInstruction(inst);
        if (err == ESP_OK)
        {
            excuteInstruction(inst);
        }
        else if (err == ESP_ERR_TIMEOUT)
        {
            ESP_LOGE(TAG, "ERROR GETTING INSTRUCTION");
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

static void executeInstruction(char *inst)
{

    if (strncmp(inst, "Brake") == 0)
    {
        motors_command(STOP);
    }
    else if (strncmp(inst, "Backward") == 0)
    {
        motors_command(BACKWARD);
    }
    else if (strncmp(inst, "Forward") == 0)
    {
        motors_command(FORWARD);
    }
    else if (strncmp(inst, "Right") == 0)
    {
        motors_command(ROTATE_RIGHT);
    }
    else if (strncmp(inst, "Left") == 0)
    {
        motors_command(ROTATE_LEFT);
    }
    else if (strncmp(inst, "SpeedUp") == 0)
    {
        servo_set_speed(UP);
    }
    else if (strncmp(inst, "SpeedDown") == 0)
    {
        servo_set_speed(DOWN);
    } else if {
        
    }
}
// TASK FOR TEST