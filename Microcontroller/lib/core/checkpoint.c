#include "checkpoint.h"
#include <setjmp.h>
#include "esp_log.h"

static char *TAG = "CHECKPOINT";
static jmp_buf checkpoint;

esp_err_t restart_semaphores(void);

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
    //abort_tasks();
    //LIMPIANDO MEMORIA (dinamica y buffer)
    //free_memory();
    //clean_buffer();
    //REINICIANDO SEMAFOROS
    //restart_semaphores();
    //ABORTANDO
    longjmp(checkpoint, 1);
}

/* esp_err_t restart_semaphores()
{
    esp_err_t err;
    err = i2c_delete_bus();  // i2c
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete the semaphore controlling the I2C bus.");
        return err;
    }

    err = delete_buffer_semaphore();  // instruction_buffer
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete the semaphore to access the buffer.");
        return err;
    }

    err = delete_flag_semaphore();  // limit_switch
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete the semaphore ffor the limit switch.");
        return err;
    }

    err = delete_servo_semaphores();  // servo
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete the semaphores used for the servo.");
        return err;
    }

    return ESP_OK;
}
*/