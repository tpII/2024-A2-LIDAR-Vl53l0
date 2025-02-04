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

#endif // DEBUG_HELPER_H_
