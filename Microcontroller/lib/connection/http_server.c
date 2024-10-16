#include "http_server.h"

static esp_err_t home_get_handler(httpd_req_t *req);
static esp_err_t post_handler(httpd_req_t *req);

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
        httpd_uri_t uri_post = {
            .uri = "/post",
            .method = HTTP_POST,
            .handler = post_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &uri_post);
    }
}

// handlers

static esp_err_t home_get_handler(httpd_req_t *req) {
    const char* resp_str = "CYCLOPS";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}

static esp_err_t post_handler(httpd_req_t *req) {
    // Leer el cuerpo del mensaje POST
    char content[100];
    int ret = httpd_req_recv(req, content, sizeof(content));
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);  // Timeout
        }
        return ESP_FAIL;
    }
    
    // Deserializar JSON para obtener el tipo de mensaje
    cJSON *json = cJSON_Parse(content);
    if (json == NULL) {
        httpd_resp_send_500(req);  // Error en JSON
        return ESP_FAIL;
    }

    // Obtener el tipo de mensaje
    const cJSON *message_type = cJSON_GetObjectItem(json, "message_type");
    if (cJSON_IsString(message_type) && (message_type->valuestring != NULL)) {
        // Comparar el tipo de mensaje y manejar en consecuencia
        if (strcmp(message_type->valuestring, "tipo1") == 0) {
            // Manejar tipo de mensaje 1
            httpd_resp_sendstr(req, "Mensaje tipo 1 recibido");
        } else if (strcmp(message_type->valuestring, "tipo2") == 0) {
            // Manejar tipo de mensaje 2
            httpd_resp_sendstr(req, "Mensaje tipo 2 recibido");
        } else {
            // Tipo de mensaje desconocido
            httpd_resp_sendstr(req, "Tipo de mensaje no reconocido");
        }
    }
    
    cJSON_Delete(json);
    return ESP_OK;
}