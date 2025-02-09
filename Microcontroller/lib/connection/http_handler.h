/**
 * @file http_handler.h
 * @author Guerrico Leonel (lguerrico99@gmail.com)
 * @date 2025-02-09
 * @brief Module for handling instruction retrieval via HTTP.
 * 
 * This file defines functions to send HTTP GET requests and retrieve the latest 
 * instruction from a remote server in an ESP32 environment.
 */

 #ifndef _HTTP_HANDLER_H_
 #define _HTTP_HANDLER_H_
 
 #include "esp_err.h"
 
 /**
  * @brief Performs an HTTP GET request to retrieve the latest available instruction.
  *
  * This function connects to a remote HTTP server and fetches the latest instruction
  * in JSON format. The instruction is then processed and stored in a buffer for 
  * further execution.
  *
  * @return 
  * - ESP_OK: If the request was successful.
  * - ESP_FAIL: If there was an error initializing the HTTP client or in the communication.
  */
 esp_err_t getHTTPInstruction(void);
 
 #endif // _HTTP_HANDLER_H_
 