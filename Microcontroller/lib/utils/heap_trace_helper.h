#ifndef HEAP_TRACE_HELPER_H
#define HEAP_TRACE_HELPER_H

#include "esp_heap_trace.h"
#include "esp_err.h"

#define NUM_RECORDS 100

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Inicia el heap tracing en modo LEAKS.
 * 
 * Llama a `heap_trace_init_standalone()` para configurar el buffer de registros
 * y luego inicia el trace con `heap_trace_start(HEAP_TRACE_LEAKS)`.
 * 
 * @return esp_err_t ESP_OK en caso de éxito, o un código de error si falla.
 */
esp_err_t start_heap_trace(void);

/**
 * @brief Detiene el heap tracing y muestra los resultados en la consola.
 * 
 * Llama a `heap_trace_stop()` para detener la captura y luego usa `heap_trace_dump()`
 * para imprimir los registros almacenados.
 */
void stop_heap_trace(void);

#ifdef __cplusplus
}
#endif

#endif // HEAP_TRACE_HELPER_H
