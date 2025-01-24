/**
 * @file instruction_buffer.c
 * @brief Implementation of a circular buffer for storing instructions.
 * 
 * This file contains the implementation of a circular buffer that allows storing
 * and retrieving instructions in a thread-safe manner. The buffer uses FreeRTOS
 * semaphores to prevent concurrent access issues.
 * 
 * @version 1.0
 * @date 2024-12-05
 * 
 * @note
 * - The buffer has a fixed size of 10 instructions, with a maximum instruction
 *   length of 40 characters.
 * - Ensure `initBuffer` is called before `saveInstruction` or `getInstruction`.
 */
#include "instruction_buffer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <string.h>
#include "esp_log.h"


#define INSTRUCTIONS_BUFFER_SIZE 10 ///< Number of instructions the buffer can hold.
#define INSTRUCTION_MAX_LENGTH 40   ///< Maximum length of each instruction.

// Global Variables
char instructions_buffer[INSTRUCTIONS_BUFFER_SIZE][INSTRUCTION_MAX_LENGTH]; ///< Circular buffer.
static uint8_t push_index = 0;  ///< Index for the next instruction to be saved.
static uint8_t get_index = 0;   ///< Index for the next instruction to be retrieved.
static SemaphoreHandle_t buffer_access;         ///< Semaphore for thread-safe access.
static const char *TAG = "INSTRUCTION_BUFFER";  ///< Tag for logging.


/**
 * @brief Initializes the instruction buffer.
 * 
 * Creates the semaphore required for synchronizing access to the buffer. 
 * Logs an error if the semaphore creation fails.
 * 
 * @return 
 * - `ESP_OK`: If the buffer was successfully initialized.
 * - `ESP_FAIL`: If semaphore creation fails.
 */
esp_err_t initBuffer()
{
    if (buffer_access == NULL) {
        buffer_access = xSemaphoreCreateMutex();
        if (buffer_access == NULL) {
            ESP_LOGE(TAG,"Error creating Semaphore");
            return ESP_FAIL;
        }
    }
    return ESP_OK;
}

/**
 * @brief Retrieves the next instruction from the buffer.
 * 
 * Fetches the oldest instruction in the buffer and removes it. If the buffer is 
 * empty, the function returns an error.
 * 
 * @param[out] inst Pointer to a buffer where the instruction will be stored.
 * 
 * @return 
 * - `ESP_OK`: If the instruction was successfully retrieved.
 * - `ESP_FAIL`: If the buffer is empty or the semaphore cannot be acquired.
 */
esp_err_t getInstruction(char *inst)
{   
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

/**
 * @brief Saves a new instruction into the buffer.
 * 
 * Stores a new instruction in the buffer. If the buffer is full, the function 
 * returns an error.
 * 
 * @param[in] inst Pointer to the instruction to be saved.
 * 
 * @return 
 * - `ESP_OK`: If the instruction was successfully saved.
 * - `ESP_FAIL`: If the buffer is full or the semaphore cannot be acquired.
 */
esp_err_t saveInstruction(char *inst)
{

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