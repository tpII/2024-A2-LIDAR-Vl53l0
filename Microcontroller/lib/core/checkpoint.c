/**
 * @file checkpoint.c
 * @author
 *      Ossola Florencia (flor.ossola13@gmail.com)
 * @brief Implementation of an incomplete checkpoint system.
 *
 * This file attempts to provide a rollback mechanism for execution
 * using `setjmp()` and `longjmp()`. However, the resource cleanup
 * and task abortion processes are not fully implemented.
 *
 * WARNING: This implementation is INCOMPLETE and does not
 * guarantee proper cleanup before rollback.
 *
 * @date 2025-02-09
 * @version 0.1 (INCOMPLETE)
 */

#include "checkpoint.h"
#include <setjmp.h>
#include "esp_log.h"
#include "debug_helper.h"

static char *TAG = "CHECKPOINT";
static jmp_buf checkpoint;

/**
 * @brief Deletes semaphores to release resources.
 *
 * WARNING: This function attempts to delete various semaphores
 * but does not ensure full recovery. Use with caution.
 *
 * @return esp_err_t ESP_OK if all semaphores are deleted successfully,
 * or an error code if any deletion fails.
 */
esp_err_t delete_semaphores(void);

/**
 * @brief Sets a checkpoint for rollback.
 *
 * Uses `setjmp()` to create a checkpoint. If `longjmp()` is
 * called, execution returns to this checkpoint.
 *
 * @return esp_err_t ESP_OK if successful, ESP_FAIL if checkpoint
 * creation fails.
 */
esp_err_t setCheckpoint()
{
    ESP_LOGI(TAG, "Setting checkpoint...");
    int err = setjmp(checkpoint);
    if (err)
    {
        ESP_LOGE(TAG, "Error setting checkpoint");
        LOG_MESSAGE_E(TAG, "Error setting checkpoint");
        return ESP_FAIL;
    }
    return ESP_OK;
}

/**
 * @brief Activates rollback to the last checkpoint.
 *
 * WARNING: This function is incomplete. It is meant to:
 * - Abort tasks (not implemented)
 * - Clean memory (not needed but not implemented)
 * - Clear buffers (partially implemented)
 * - Reset semaphores (partially implemented)
 * - Trigger rollback with `longjmp()`
 *
 * Calling this function will attempt to return execution to the
 * last checkpoint but may not clean up resources properly.
 */
void activeCheckpoint()
{
    ESP_LOGI(TAG, "Activating checkpoint...");
    // TASK TERMINATION (NOT IMPLEMENTED)
    // abort_tasks();

    // MEMORY CLEANUP (MALLOC NOT USED, NOT IMPLEMENTED)
    // free_memory();

    // BUFFER CLEARING (PLANNED BUT NOT IMPLEMENTED)
    // clearBuffer();

    // SEMAPHORE RESET (PARTIALLY IMPLEMENTED)
    // delete_semaphores();

    // ROLLBACK EXECUTION
    longjmp(checkpoint, 1);
}

/**
 * @brief Deletes semaphores used in various components.
 * 
 * WARNING: This function is incomplete and may not properly 
 * release all system resources.
 * 
 * @return esp_err_t ESP_OK if successful, or an error code if any deletion fails.
 */
esp_err_t delete_semaphores()
{
    esp_err_t err;
    err = i2c_delete_bus(); // i2c
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete the semaphore controlling the I2C bus.");
        LOG_MESSAGE_E(TAG, "Failed to delete the semaphore controlling the I2C bus.");
        return err;
    }

    err = delete_buffer_semaphore(); // instruction_buffer
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete the semaphore to access the buffer.");
        LOG_MESSAGE_E(TAG, "Failed to delete the semaphore to access the buffer.");
        return err;
    }

    err = delete_flag_semaphore(); // limit_switch
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete the semaphore ffor the limit switch.");
        LOG_MESSAGE_E(TAG, "Failed to delete the semaphore ffor the limit switch.");
        return err;
    }

    err = delete_servo_semaphores(); // servo
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete the semaphores used for the servo.");
        LOG_MESSAGE_E(TAG, "Failed to delete the semaphores used for the servo.");
        return err;
    }

    return ESP_OK;
}
