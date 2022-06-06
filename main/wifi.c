#include "wifi.h"
#include "config.h"
#include "utils.h"
#include "publish.h"

#define MAX_RETRY 10
static const char *TAG = "wifi_service";

static esp_err_t handle_http_event(esp_http_client_event_t *);


wifi_err_t post_data(sensor_values_t v)
{ 
    char post_data[300];
    wifi_err_t sent = 0;

    sprintf(post_data, 
    "{\"ext_temp\" : %.2f, "
    "\"internal_temp\" : %.2f, "
    "\"humidity\" : %.2f, "
    "\"baro_pressure\" : %.2f, "
    "\"wind_speed\" : %.2f, "
    "\"wind_direction\" : %.2f }"
    , v.ext_temp, v.temp, v.humidity, v.baro_pressure, v.wind_speed, v.wind_direction);

    // char* cert = read_tls_cert();
    // if (!cert){
    //     printf("TLS certificate not in namespace. Aborting...\n");
    //     return;
    // }

    esp_http_client_config_t config = {
        .url = HOSTNAME,
        .event_handler = handle_http_event,
        .method = HTTP_METHOD_POST,
        // .cert_pem = ca_cert
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    printf("%s\n", post_data);

    for(int i=0; i < MAX_RETRY; ++i)
    {
        if (esp_http_client_perform(client) != ESP_OK)
            ESP_LOGE(TAG, "http request failed");
        else
        {
            sent = 1;
            break;
        }
    }
    esp_http_client_cleanup(client);

    return sent;
}

static esp_err_t handle_http_event(esp_http_client_event_t *http_event)
{
    switch (http_event->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        printf("%.*s\n", http_event->data_len, (char *)http_event->data);
        break;
    default:
        break;
    }
    return ESP_OK;
}


