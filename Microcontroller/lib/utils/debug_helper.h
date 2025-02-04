#ifndef DEBUG_HELPER_H_
#define DEBUG_HELPER_H_

// Si DEBUG no está definido en otro lado, puedes definirlo aquí
#ifndef DEBUG
    #define DEBUG 0  // Puedes cambiarlo a 0 para desactivar los logs
#endif

#ifdef DEBUG
    #define DEBUGING_ESP_LOG(...) __VA_ARGS__
#else
    #define DEBUGING_ESP_LOG(...)
#endif

#endif // DEBUG_HELPER_H_
