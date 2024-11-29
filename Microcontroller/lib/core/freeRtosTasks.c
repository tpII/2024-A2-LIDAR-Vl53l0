#include "freeRtosTasks.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "servo_interruptions.h"

static void servoInterruptionTask(void *);

TaskHandle_t servoInterruptionTaskHandler = NULL;

esp_err_t createTasks()
{
    // BACKGROUND TASKs
    BaseType_t task_created = xTaskCreatePinnedToCore(
        servoInterruptionTask, // Función de la tarea
        "ServoInterruptionTask",    // Nombre de la tarea
        2048,                       // Tamaño de la pila
        NULL,                       // Argumento de la tarea (NULL si no es necesario)
        1,                          // Prioridad
        &servoInterruptionTaskHandler,                       // Puntero al handle de la tarea (NULL si no es necesario)
        tskNO_AFFINITY              // Núcleo (sin afinidad específica)
    );

    // MAIN TASKs

    if (task_created != pdPASS)
    {
        return ESP_FAIL; // Retorna error si la tarea no se pudo crear
    }

    return ESP_OK; // Retorna éxito si la tarea fue creada correctamente
    
}

void abort_tasks(){
    if(servoInterruptionTaskHandler != NULL){
        vTaskDelete(servoInterruptionTaskHandler);
        servoInterruptionTaskHandler = NULL;
    }

    //REST OF TASKS
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

//TASK FOR TEST