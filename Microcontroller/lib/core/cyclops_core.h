/**
 * @file cyclops_core.h
 * @author Guerrico Leonel (lguerrico@outlook.com), Ossola Florencia (flor.ossola13@gmail.com) 
 * and Octavio Perez Balcedo
 * 
 * @brief Core system initialization and task management for the Cyclops project.
 *
 * This module provides functions for initializing the core services,
 * creating tasks, and aborting tasks within the Cyclops system.
 *
 * @date 2025-02-09
 * @version 1.0
 */
#ifndef _CYCLOPS_CORE_H_
#define _CYCLOPS_CORE_H_

#include "esp_err.h"
#include "http_handler.h"
#include "instruction_buffer.h"

/**
 * @brief Initializes the core system components.
 *
 * This function initializes the required services including the server, MQTT,
 * motors, mapping, lights, and battery monitoring.
 *
 * @return ESP_OK if initialization succeeds, otherwise an error code.
 */
esp_err_t system_init(void);

/**
 * @brief Creates the necessary tasks for system operation.
 *
 * This function spawns various tasks such as handling instructions,
 * receiving instructions, battery monitoring, and mapping services.
 *
 */ 
esp_err_t createTasks(void);

/**
 * @brief Aborts all running tasks.
 *
 * This function deletes all running tasks and resets their handles to NULL.
 */
void abort_tasks(void);

#endif  /* _CYCLOPS_CORE_H_ */