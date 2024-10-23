#include "instruction_buffer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "json_helper.h"
#include <string.h>
#include "esp_log.h"

#define INSTRUCTIONS_BUFFER_SIZE 10
#define INSTRUCTION_MAX_LENGTH 40

char instructions_buffer[INSTRUCTIONS_BUFFER_SIZE][INSTRUCTION_MAX_LENGTH];

static uint8_t push_index = 0;
static uint8_t get_index = 0;
static SemaphoreHandle_t buffer_access;
static const char *TAG = "INSTRUCTION_BUFFER";

static void initBuffer(void);

static void initBuffer()
{
    if (buffer_access == NULL) {
        buffer_access = xSemaphoreCreateMutex();
    }
}


esp_err_t getInstruction(char *inst)
{   
    initBuffer();
    // Toma el semáforo antes de acceder al buffer
    if (xSemaphoreTake(buffer_access, portMAX_DELAY) == pdTRUE)
    {
        // Comprobar si el buffer está vacío
        if (get_index == push_index)
        {
            xSemaphoreGive(buffer_access); // Liberar el semáforo
            return ESP_FAIL;               // Buffer vacío
        }

        // Copiar la instrucción en la posición de get_index
        strncpy(inst, instructions_buffer[get_index], INSTRUCTION_MAX_LENGTH);

        // Actualizar get_index de forma cíclica
        get_index = (get_index + 1) % INSTRUCTIONS_BUFFER_SIZE;

        // Liberar el semáforo después de acceder al buffer
        xSemaphoreGive(buffer_access);
        return ESP_OK;
    }

    return ESP_FAIL;
}

esp_err_t saveInstruction(char *inst)
{
    initBuffer();

    // Toma el semáforo antes de acceder al buffer
    if (xSemaphoreTake(buffer_access, portMAX_DELAY) == pdTRUE)
    {
        // Verificar si el buffer está lleno
        uint8_t next_push_index = (push_index + 1) % INSTRUCTIONS_BUFFER_SIZE;
        if (next_push_index == get_index)
        {
            xSemaphoreGive(buffer_access); // Liberar el semáforo
            return ESP_FAIL;               // Buffer lleno
        }

        // Copiar la instrucción a la posición de push_index
        strncpy(instructions_buffer[push_index], inst, INSTRUCTION_MAX_LENGTH);

        // Actualizar push_index de forma cíclica
        push_index = next_push_index;

        // Liberar el semáforo después de acceder al buffer
        xSemaphoreGive(buffer_access);
        return ESP_OK;
    }

    return ESP_FAIL;
}