#include "mapping.h"
#include "servo.h"
#include "vl53l0x.h"
#include "esp_log.h"

#define MIN_DISTANCE 100

static const char *TAG = "MAPPING";
static esp_err_t getValue(uint16_t *);

// Función de comparación para ordenar de menor a mayor
uint16_t compare(const void *a, const void *b) {
    return (*(uint16_t*)a - *(uint16_t*)b);
}

esp_err_t mapping_init()
{

    esp_err_t err = ESP_OK;
    //ESP_LOGI(TAG, "Initializing Mapping");

    //ESP_LOGI(TAG, "Initializing GPIO...");
    
    err = gpio_init();
    ESP_RETURN_ON_ERROR(err, TAG, "Error in gpio_init: %s", esp_err_to_name(err));
    // if (err != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "Error in gpio_init: %s", esp_err_to_name(err));
    //     return err;
    // }

    //ESP_LOGI(TAG, "Initializing I2C...");
    err = i2c_init();
    ESP_RETURN_ON_ERROR(err, TAG, "Error initializing I2C: %s", esp_err_to_name(err));
    // if (err != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "Error initializing I2C");
    //     return ESP_FAIL;
    // }
    
    //ESP_LOGI(TAG, "Initializing LiDAR...");
    if (!vl53l0x_init())
    {
        ESP_LOGE(TAG, "Error initializing LiDAR(VL53L0X)");
        return ESP_FAIL;
    }

    //ESP_LOGI(TAG, "Initializing ServoMotor...");
    err = servo_initialize();
    ESP_RETURN_ON_ERROR(err, TAG, "Error initializing Servo: %s", esp_err_to_name(err));
    // if (err != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "Error initializing Servo");
    //     return ESP_FAIL;
    // }
    
    err = servo_start();
    ESP_RETURN_ON_ERROR(err, TAG, "Error Starting Servo: %s", esp_err_to_name(err));
    // if (err != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "Error Starting Servo");
    //     return ESP_FAIL;
    // }

    return err;
}

esp_err_t getMappingValue(uint16_t *angle, uint16_t *distance)
{

    if (angle == NULL || distance == NULL)
    {
        ESP_LOGE(TAG, "NULL pointer passed to getMappingValue.");
        return ESP_ERR_INVALID_ARG;
    }

    *angle = readAngle();
    esp_err_t err = getValue(distance);
    ESP_RETURN_ON_ERROR(err, TAG, "ERROR MAPPING: %s", esp_err_to_name(err));
    // if( err != ESP_OK){
    //     ESP_LOGW(TAG,"ERROR MAPPING: %s",esp_err_to_name(err));
    //     return err;
    // }
    return ESP_OK;     
}

static esp_err_t getValue(uint16_t *distance)
{
    bool success = false;
    uint16_t val = 0;
    uint16_t values[10];
    uint8_t valid_readings = 0; // Contador para lecturas válidas

    for (uint8_t i = 0; i < 10; i++)
    {
#ifndef VL53L0X
        success = vl53l0x_read_range_single(VL53L0X_IDX_FIRST, &val);
        if (success && (val != VL53L0X_OUT_OF_RANGE || val >= MIN_DISTANCE))
        {
            values[i] = val;  // Almacenar el valor si es válido
            valid_readings++; // Incrementar el contador de lecturas válidas
        }
        else
        {
            // Si la lectura no es exitosa o el valor está fuera de rango
            ESP_LOGE(TAG, "Error reading range or invalid value: %d", val);
        }
#else
        ESP_LOGE(TAG, "ERROR VL53L0X NOT DEFINED");
        return ESP_FAIL; // Si VL53L0X no está definido, retornar error
#endif
    }
    // Ordenar las mediciones de menor a mayor
    // Verificar si se obtuvieron suficientes lecturas válidas
    if (valid_readings < 6)
    {
        ESP_LOGE(TAG, "Not enough valid readings. Only %d valid readings.", valid_readings);
        return ESP_ERR_INVALID_ARG; // Retornar error si no se obtuvieron suficientes mediciones válidas
    }

    // Ordenar las mediciones de menor a mayor
    // for (uint8_t i = 0; i < valid_readings - 1; i++)
    // {
    //     for (uint8_t j = i + 1; j < valid_readings; j++)
    //     {
    //         if (values[i] > values[j])
    //         {
    //             // Intercambiar los valores
    //             uint16_t temp = values[i];
    //             values[i] = values[j];
    //             values[j] = temp;
    //         }
    //     }
    // }

    qsort(values, valid_readings, sizeof(uint16_t), compare);

    // La mediana será el valor en la posición central del arreglo ordenado
    *distance = values[valid_readings / 2];
    return ESP_OK; // Índice 5 es la mediana en un arreglo de 10 elementos
}
esp_err_t mapping_pause()
{
    esp_err_t err = ESP_OK;

    err = servo_pause();
    ESP_RETURN_ON_ERROR(err, TAG, "ERROR STOPING SERVO: %s", esp_err_to_name(err));
    // if (err != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "ERROR STOPING SERVO");
    //     return ESP_FAIL;
    // }
    return ESP_OK;
}

esp_err_t mapping_stop()
{
    return servo_stop();
}

esp_err_t mapping_restart()
{
    esp_err_t err = ESP_OK;

    err = servo_restart();
    ESP_RETURN_ON_ERROR(err, TAG, "ERROR RESTARTING SERVO: %s", esp_err_to_name(err));
    // if (err != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "ERROR RESTARTING SERVO");
    //     return ESP_FAIL;
    // }
    return ESP_OK;
}