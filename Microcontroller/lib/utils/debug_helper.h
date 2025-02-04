#ifndef DEBUG_HELPER_H_
#define DEBUG_HELPER_H_

// Si DEBUG no está definido en otro lado, puedes definirlo aquí
#ifndef DEBUG
    #define DEBUG 0  // Puedes cambiarlo a 0 para desactivar los logs
#endif

#if DEBUG
    #define DEBUGING_ESP_LOG(...) do { __VA_ARGS__; } while (0)
#else
    #define DEBUGING_ESP_LOG(...) do {} while (0)
#endif

void LOG_MESSAGE(int level, char *TAG, char *fmt);

// Definir macros de nivel de log
#define LOG_ERROR   1
#define LOG_WARNING 2
#define LOG_INFO    3

//#define LOG_MESSAGE_E(TAG, fmt, ...) LOG_MESSAGE(LOG_ERROR, TAG, fmt, ##__VA_ARGS__)
#define LOG_MESSAGE_E(TAG, fmt) LOG_MESSAGE(LOG_ERROR, TAG, fmt)
#define LOG_MESSAGE_W(TAG, fmt) LOG_MESSAGE(LOG_WARNING, TAG, fmt)
#define LOG_MESSAGE_I(TAG, fmt) LOG_MESSAGE(LOG_INFO, TAG, fmt)

#endif // DEBUG_HELPER_H_
