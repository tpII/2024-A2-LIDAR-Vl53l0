#ifndef _SERVOMOTOR2_H_
#define _SERVOMOTOR2_H_

#include "esp_err.h"

/**
 * @brief Initialize the servo on GPIO16 with a pulse width range of 600us to 2100us.
 *
 * @return 
 *    - ESP_OK on success
 *    - ESP_FAIL on error
 */
esp_err_t initialize_servo(void);

/**
 * @brief Set the angle of the servo (0 to 180 degrees).
 *
 * @param angle Desired angle in degrees.
 * @return 
 *    - ESP_OK on success
 *    - ESP_ERR_INVALID_ARG if the angle is out of range
 *    - ESP_FAIL on other errors
 */
esp_err_t write_angle(float angle);

/**
 * @brief Read the current angle of the servo by calculating from the duty cycle.
 *
 * @param angle Pointer to store the current angle.
 * @return 
 *    - ESP_OK on success
 *    - ESP_ERR_INVALID_ARG if the angle pointer is invalid
 *    - ESP_FAIL on other errors
 */
esp_err_t read_angle(float *angle);


#endif