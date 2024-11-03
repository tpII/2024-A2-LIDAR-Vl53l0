#include "mqtt_server.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "json_helper.h"
#include "instruction_buffer.h"
#include "esp_system.h"

#define NUM_TOPICS 4

static const char *TAG = "MQTT_SERVER";
static const char *URL = "mqtt://192.168.4.2:1883";
static esp_mqtt_client_handle_t mqtt_client = NULL;
static const char *TOPICS[] = {"Instruction", "Messages", "Mapping", "Battery"};
static char inst[40] = {0};
static uint32_t MQTT_CONNEECTED = 0;

static esp_err_t mqtt_connect(void);
static esp_err_t mqtt_subscribe(const char *);
// static esp_err_t mqtt_event_handler(esp_mqtt_event_t *);
static void mqtt_event_handler(void *, esp_event_base_t, int32_t, void *);
static void mqtt_subscribing(void);
static void instruction_handler(char *, size_t length);

static esp_err_t mqtt_connect()
{
    ESP_LOGI(TAG, "INITIALIZING MQTT");
    const esp_mqtt_client_config_t mqtt_config = {
        .broker.address.uri = URL,

    };
    mqtt_client = esp_mqtt_client_init(&mqtt_config);
    if (mqtt_client == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return ESP_FAIL;
    }

    // Registrar el manejador de eventos
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, mqtt_client);

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
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        MQTT_CONNEECTED = 1;
        mqtt_subscribing();
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        MQTT_CONNEECTED = 0;
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        if (strncmp(event->topic, "Instruction", event->topic_len) == 0)
        {
            ESP_LOGI(TAG, "Instruction received");

            instruction_handler(event->data, event->data_len);
        }
        else
        {
            ESP_LOGW(TAG, "ERROR: Message received from an unexpected Topic(%s)", event->topic);
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

// Función para publicar un mensaje en un tópico
esp_err_t mqtt_publish(const char *topic, const char *payload)
{
    if (mqtt_client == NULL)
    {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_FAIL;
    }

    int msg_id = esp_mqtt_client_publish(mqtt_client, topic, payload, 0, 1, 0);
    if (msg_id == -1)
    {
        ESP_LOGE(TAG, "Failed to publish message to topic %s", topic);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Published message to topic %s", topic);
    return ESP_OK;
}

// Función para suscribirse a un tópico
static esp_err_t mqtt_subscribe(const char *topic)
{
    if (mqtt_client == NULL)
    {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_FAIL;
    }

    int msg_id = esp_mqtt_client_subscribe(mqtt_client, topic, 0);
    if (msg_id == -1)
    {
        ESP_LOGE(TAG, "Failed to subscribe to topic %s", topic);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Subscribed to topic %s", topic);
    return ESP_OK;
}

// Función para desconectar el cliente MQTT
esp_err_t mqtt_disconnect()
{
    if (mqtt_client == NULL)
    {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return ESP_FAIL;
    }

    esp_err_t err = esp_mqtt_client_stop(mqtt_client);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to stop MQTT client: %s", esp_err_to_name(err));
        return err;
    }

    mqtt_client = NULL; // Limpiar el puntero del cliente
    ESP_LOGI(TAG, "MQTT client disconnected");
    return ESP_OK;
}

esp_err_t mqtt_start()
{

    esp_err_t err = mqtt_connect();
    uint8_t retry_count = 0;
    const uint8_t max_retries = 5;
    while (err != ESP_OK && retry_count < max_retries)
    {
        ESP_LOGW(TAG, "Reconnecting... attempt %d", retry_count + 1);
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Espera 2 segundos antes de reintentar
        err = mqtt_connect();
        retry_count++;
    }
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to connect to MQTT after %d retries", max_retries);
        return err;
    }

    return ESP_OK;
}

static void mqtt_subscribing()
{
    esp_err_t err2;
    for (uint8_t i = 0; i < NUM_TOPICS; i++)
    {
        err2 = mqtt_subscribe(TOPICS[i]);
        if (err2 != ESP_OK)
        {
            ESP_LOGW(TAG, "Error subscribing to topic: %s", TOPICS[i]);
        }
        else
        {
            ESP_LOGI(TAG, "Successful subscription to: %s", TOPICS[i]);
        }
    }
}

static void instruction_handler(char *str, size_t length)
{

    memset(inst, 0, sizeof(inst));
    char *message = (char *)malloc(length + 1);
    if (message == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for MQTT message");
    }

    // Copiar los datos y agregar el terminador nulo manualmente
    memcpy(message, str, length);
    message[length] = '\0'; // Termina la cadena con '\0'
    esp_err_t err = deserealize_json_data(message, inst, length);
    free(message);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error deserializing Instruction");
    }
    else
    {

        if (strncmp(inst, "REBOOT", strlen("REBOOT")) == 0)
        {
            ESP_LOGW("RESTART", "Reiniciando el MCU...");
            esp_restart(); // Llamada para reiniciar el MCU
        }
        else
        {
            if (saveInstruction(inst) != ESP_OK)
            {
                ESP_LOGE(TAG, "Error saving Instruction");
            }
        }
    }
}