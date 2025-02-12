/**
 * @file instruction_buffer.h
 * @author Guerrico Leonel (lguerrico@outlook.com)
 * @brief Circular buffer for storing and retrieving instructions.
 *
 * This library provides an interface for managing a circular buffer that stores
 * textual instructions. It supports operations to initialize the buffer, save
 * instructions, and retrieve them in a thread-safe manner using FreeRTOS semaphores.
 *
 * @version 1.0
 * @date 2024-12-05
 *
 * @note
 * - Ensure to call `initBuffer` before using any other functions in this library.
 * - The buffer has a fixed size of 10 instructions, with each instruction
 *   limited to 40 characters.
 *
 */

#ifndef _INSTRUCTION_BUFFER_H_
#define _INSTRUCTION_BUFFER_H_

#include "esp_err.h"

/**
 * @brief Initializes the instruction buffer.
 *
 * This function creates the semaphore required to manage access to the circular
 * buffer. It must be called before any other operations on the buffer.
 *
 * @return
 * - `ESP_OK`: If the buffer was successfully initialized.
 * - `ESP_FAIL`: If semaphore creation fails.
 */
esp_err_t initBuffer(void);

/**
 * @brief Retrieves the next instruction from the buffer.
 *
 * This function fetches the oldest instruction from the buffer and removes it.
 * If the buffer is empty, the operation fails.
 *
 * @param[out] inst Pointer to a buffer where the retrieved instruction will be stored.
 *             The buffer must be large enough to hold 40 characters.
 *
 * @return
 * - `ESP_OK`: If the instruction was successfully retrieved.
 * - `ESP_ERR_TIMEOUT`: If the buffer is empty or the operation fails.
 * - `ESP_ERR_NOT_FOUND`: If there is not new instruction.
 * 
 */
esp_err_t getInstruction(char *);

/**
 * @brief Saves a new instruction into the buffer.
 *
 * This function stores a new instruction in the buffer. If the buffer is full,
 * the operation fails.
 *
 * @param[in] inst Pointer to the instruction to be saved. The instruction must be
 *             null-terminated and no longer than 40 characters.
 *
 * @return
 * - `ESP_OK`: If the instruction was successfully saved.
 * - `ESP_FAIL`: If the buffer is full or the operation fails.
 */
esp_err_t saveInstruction(char *);

/**
 * @brief Deletes the semaphore used for buffer access control.
 *
 * This function deletes the semaphore responsible for synchronizing access 
 * to the buffer. It ensures safe deletion by checking if the semaphore exists.
 *
 * @return 
 *      - ESP_OK if the semaphore was successfully deleted.
 *      - ESP_FAIL if the semaphore was NULL before deletion.
 */
esp_err_t delete_buffer_semaphore();

/**
 * @brief Clears the instruction buffer and resets its indexes.
 *
 * This function resets all elements in the `instructions_buffer` array to 0 
 * and reinitializes the push and get indexes. It ensures safe access 
 * using a semaphore with a timeout.
 *
 * @return 
 *      - ESP_OK if the buffer was successfully cleared.
 *      - ESP_ERR_TIMEOUT if acquiring the semaphore failed within 50ms.
 */
esp_err_t clearBuffer(void);

#endif