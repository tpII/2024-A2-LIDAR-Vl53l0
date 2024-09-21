#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

#include "esp_err.h"

//esp_err_t http_handler_init(void);
esp_err_t http_post(const char *, const char **, const char **, const size_t);
//esp_err_t http_put(const char *message_type);
esp_err_t http_get(const char *, char *, const size_t );
//esp_err_t http_delete(const char *message_type);



#endif //HTTP_CLIENT_H_