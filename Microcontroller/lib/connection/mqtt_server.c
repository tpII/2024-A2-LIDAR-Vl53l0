#include "mqtt_server.h"
#include "esp_log.h"
#include "mqtt_client.h"

static const char *TAG = "MQTT_SERVER";
static const char *URL = "mqtt://192.168.4.2:1883";
static const char *ID = "ESP32";
static esp_mqtt_client_handle_t mqtt_client = NULL;
static const char *TOPICS[] = {"Instruction","Messages","Mapping","Battery"};

static esp_err_t mqtt_connect(void);
static esp_err_t mqtt_subscribe(const char *);
static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t); 

static esp_err_t mqtt_connect()
{
    ESP_LOGI(TAG,"INITIALIZING MQTT");
    esp_mqtt_client_config_t mqtt_config = {
        .broker = {
            .address = {
                .uri = URL, // URI del broker                     // Puerto del broker
            },
        },
        .credentials = {
            .username = NULL,                  // Sin nombre de usuario
            .client_id = ID,                   // ID de cliente, puede ser NULL para usar el predeterminado
            .set_null_client_id = true,        // Establecer ID de cliente como NULL
            .authentication.password = NULL,   // Sin contraseña
        },
        .session = {
            .keepalive = 120,                  // Tiempo de keepalive en segundos
        },
        .network = {
            .reconnect_timeout_ms = 10000,     // Timeout de reconexión en ms
            .timeout_ms = 10000,                // Timeout de operación en ms
        },
        .task = {
            .priority = 5,                      // Prioridad de la tarea
            .stack_size = 4096,                 // Tamaño de la pila
        },
        .buffer = {
            .size = 1024,                       // Tamaño del buffer de entrada/salida
            .out_size = 512,                    // Tamaño del buffer de salida
        },
        .outbox = {
            .limit = 4096,                      // Tamaño límite del outbox en bytes
        }
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_config);
    if (mqtt_client == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return ESP_FAIL;
    }


    // Registrar el manejador de eventos
    esp_mqtt_client_register_event(mqtt_client, MQTT_EVENT_DATA, mqtt_event_handler, NULL);

    esp_err_t err = esp_mqtt_client_start(mqtt_client);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start MQTT client: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "MQTT client connected to %s", URL);
    return ESP_OK;
}

// Manejo de eventos MQTT
static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {
    switch (event->event_id) {
        case MQTT_EVENT_DATA: {
            if (event->data_len) {
                char received_data[event->data_len + 1]; // +1 para el null terminador
                memcpy(received_data, event->data, event->data_len);
                received_data[event->data_len] = '\0'; // Agregar null terminador

                ESP_LOGI(TAG, "Received message from topic %s: %s", event->topic, received_data);
            }
            break;
        }
        // Manejar otros eventos si es necesario
        default:
            break;
    }
    return ESP_OK;
}

// Función para publicar un mensaje en un tópico
esp_err_t mqtt_publish(const char *topic, const char *payload) {
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_FAIL;
    }

    int msg_id = esp_mqtt_client_publish(mqtt_client, topic, payload, 0, 1, 0);
    if (msg_id == -1) {
        ESP_LOGE(TAG, "Failed to publish message to topic %s", topic);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Published message to topic %s", topic);
    return ESP_OK;
}

// Función para suscribirse a un tópico
static esp_err_t mqtt_subscribe(const char *topic) {
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_FAIL;
    }

    int msg_id = esp_mqtt_client_subscribe(mqtt_client, topic, 0);
    if (msg_id == -1) {
        ESP_LOGE(TAG, "Failed to subscribe to topic %s", topic);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Subscribed to topic %s", topic);
    return ESP_OK;
}

// Función para desconectar el cliente MQTT
esp_err_t mqtt_disconnect() {
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_FAIL;
    }

    esp_err_t err = esp_mqtt_client_stop(mqtt_client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to stop MQTT client: %s", esp_err_to_name(err));
        return err;
    }

    mqtt_client = NULL; // Limpiar el puntero del cliente
    ESP_LOGI(TAG, "MQTT client disconnected");
    return ESP_OK;
}

esp_err_t mqtt_start(){

    int topics_size = sizeof(TOPICS) / sizeof(TOPICS[0]);
    esp_err_t err =  mqtt_connect();
    while(err != ESP_OK){
       err =  mqtt_connect();
    }
    esp_err_t err2;
    for(uint8_t i = 0; i < topics_size; i++){
        err2 = mqtt_subscribe(TOPICS[i]);
        if(err2 != ESP_OK){
            ESP_LOGE(TAG,"Error subscribing to topic: %s",TOPICS[i]);
            return err2;
        }
    }

    return ESP_OK;
}