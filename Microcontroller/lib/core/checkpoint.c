#include "checkpoint.h"
#include <setjmp.h>
#include "esp_log.h"

static char *TAG = "CHECKPOINT";
static jmp_buf checkpoint;

esp_err_t setCheckpoint()
{
    ESP_LOGI(TAG, "Setting checkpoint...");
    int err = setjmp(checkpoint);
    if (err)
    {
        ESP_LOGE(TAG, "Error setting checkpoint");
        return ESP_FAIL;
    }
    return ESP_OK;
}

//ANALIZAR EL SERVO
void activeCheckpoint()
{
    ESP_LOGI(TAG, "Activating checkpoint...");
    //PREPARANDO EL ABORTAR
    //ABORTANDO TAREAS
    //LIMPIANDO MEMORIA (dinamica y buffer)
    //REINICIANDO SEMAFOROS
    //ABORTANDO
    longjmp(checkpoint, 1);
}