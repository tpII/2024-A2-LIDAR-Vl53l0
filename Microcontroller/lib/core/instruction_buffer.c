/**
 * @file instruction_buffer.c
 * @author Guerrico Leonel (lguerrico@outlook.com)
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
#include "debug_helper.h"


#define INSTRUCTIONS_BUFFER_SIZE 10 ///< Number of instructions the buffer can hold.
#define INSTRUCTION_MAX_LENGTH 40   ///< Maximum length of each instruction.

// Global Variables
char instructions_buffer[INSTRUCTIONS_BUFFER_SIZE][INSTRUCTION_MAX_LENGTH]; ///< Circular buffer.
static uint8_t push_index = 0;                                              ///< Index for the next instruction to be saved.
static uint8_t get_index = 0;                                               ///< Index for the next instruction to be retrieved.
static SemaphoreHandle_t buffer_access;                                     ///< Semaphore for thread-safe access.
static const char *TAG = "INSTRUCTION_BUFFER";                              ///< Tag for logging.

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
    if (buffer_access == NULL)
    {
        buffer_access = xSemaphoreCreateBinary();
        xSemaphoreGive(buffer_access);
        if (buffer_access == NULL)
        {
            ESP_LOGE(TAG, "Error creating Semaphore");
            LOG_MESSAGE_E(TAG, "Error creating Semaphore");
            return ESP_FAIL;
        }
        ESP_LOGW(TAG, "Semaphore initialized");
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
    if (xSemaphoreTake(buffer_access, pdMS_TO_TICKS(50)) == pdTRUE)
    {

        // Comprobar si el buffer  está vacío
        if (get_index == push_index)
        {
            xSemaphoreGive(buffer_access); // Liberar el semáforo
            return ESP_ERR_NOT_FOUND;      // Buffer vacío
        }

        // Copiar la instrucción en la posición de get_index
        strncpy(inst, instructions_buffer[get_index], INSTRUCTION_MAX_LENGTH);

        // Actualizar get_index de forma cíclica
        get_index = (get_index + 1) % INSTRUCTIONS_BUFFER_SIZE;

        // Liberar el semáforo después de acceder al buffer
        xSemaphoreGive(buffer_access);

        return ESP_OK;
    }

    return ESP_ERR_TIMEOUT;
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
    if (xSemaphoreTake(buffer_access, pdMS_TO_TICKS(50)) == pdTRUE)
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

/**
 * @brief Deletes the semaphore used for buffer access control.
 *
 * This function safely deletes the semaphore that controls access to the buffer.
 * It checks if the semaphore exists before attempting deletion.
 *
 * @return 
 *      - `ESP_OK` if the semaphore was successfully deleted.
 *      - `ESP_FAIL` if the semaphore was `NULL` before deletion.
 */
esp_err_t delete_buffer_semaphore()
{
    if (buffer_access != NULL) {
        vSemaphoreDelete(buffer_access);
    }
    else {
        return ESP_FAIL;
    }
    return ESP_OK;
}

/**
 * @brief Clears the instruction buffer and resets indexes.
 *
 * This function sets all elements in the `instructions_buffer` array to 0 and 
 * resets the push and get indexes to their initial state. It ensures safe access 
 * using a semaphore with a timeout.
 *
 * @return 
 *      - `ESP_OK` if the buffer was successfully cleared.
 *      - `ESP_ERR_TIMEOUT` if acquiring the semaphore failed within 50ms.
 */
esp_err_t clearBuffer() 
{

    if (xSemaphoreTake(buffer_access, pdMS_TO_TICKS(50)) == pdTRUE)
    {
        memset(instructions_buffer, 0, sizeof(instructions_buffer)); // Poner todo en 0
        push_index = 0;
        get_index = 0;
        xSemaphoreGive(buffer_access);
        return ESP_OK;
    }

    return ESP_ERR_TIMEOUT;
}