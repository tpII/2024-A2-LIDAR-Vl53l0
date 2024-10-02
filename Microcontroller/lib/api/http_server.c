#include "http_server.h"

static esp_err_t home_get_handler(httpd_req_t *req) {
    const char* resp_str = "CYCLOPS";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}

void start_http_server(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t uri_get = {
            .uri = "/home",
            .method = HTTP_GET,
            .handler = home_get_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &uri_get);
    }
}
