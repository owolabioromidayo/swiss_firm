#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_http_client.h"
#include "esp_http_server.h"
#include <string.h>

#define WIFI_SSID "esp32_ap1"
#define WIFI_PWD "reconnect"
#define WIFI_CHANNEL 11
#define MAX_CONN_CNT 1
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define MAX_RETRY 10


static EventGroupHandle_t wifi_events;
static int retry_cnt = 0;
static const char *TAG = "ap-app";
static const char *TAG2 = "wifi_app";
static const char *HTML_FORM = "<html><form action=\"/\" method=\"post\">"
                               "<label for=\"ssid\">Local SSID:</label><br>"
                               "<input type=\"text\" id=\"ssid\" name=\"ssid\"><br>"
                               "<label for=\"pwd\">Password:</label><br>"
                               "<input type=\"text\" id=\"pwd\" name=\"pwd\"><br>"
                               "<input type=\"submit\" value=\"Submit\">"
                               "</form></html>";

//AP protos
static void start_webserver(void);
static esp_err_t handle_http_get(httpd_req_t *req);
static esp_err_t handle_http_post(httpd_req_t *req);
static void handle_wifi_events(void *, esp_event_base_t, int32_t, void *);

//STA protos
static void request_page(void *);
static esp_err_t handle_http_event(esp_http_client_event_t *);
static void handle_wifi_connection(void *, esp_event_base_t, int32_t, void *);


void init_wifi(void)
{
    if (nvs_flash_init() != ESP_OK)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

    esp_event_loop_create_default();
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &handle_wifi_events, NULL);

    esp_netif_init();
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .channel = WIFI_CHANNEL,
            .password = WIFI_PWD,
            .max_connection = MAX_CONN_CNT,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK},
    };
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
    esp_wifi_start();

    start_webserver();
}



//STA functions
static void handle_wifi_events(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        ESP_LOGI(TAG, "a station connected");
    }
}


static esp_err_t handle_http_get(httpd_req_t *req)
{
    return httpd_resp_send(req, HTML_FORM, HTTPD_RESP_USE_STRLEN);
}


static esp_err_t handle_http_post(httpd_req_t *req)
{
    char content[100];
    if (httpd_req_recv(req, content, req->content_len) <= 0)
    {
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "%.*s", req->content_len, content);
    return httpd_resp_send(req, "received", HTTPD_RESP_USE_STRLEN);
}


static void start_webserver(void)
{
    httpd_uri_t uri_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = handle_http_get,
        .user_ctx = NULL};

    httpd_uri_t uri_post = {
        .uri = "/",
        .method = HTTP_POST,
        .handler = handle_http_post,
        .user_ctx = NULL};

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_post);
    }
}


//AP functions
static void handle_wifi_connection(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (retry_cnt++ < MAX_RETRY)
        {
            esp_wifi_connect();
            ESP_LOGI(TAG, "wifi connect retry: %d", retry_cnt);
        }
        else
        {
            xEventGroupSetBits(wifi_events, WIFI_FAIL_BIT);
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "ip: %d.%d.%d.%d", IP2STR(&event->ip_info.ip));
        retry_cnt = 0;
        xEventGroupSetBits(wifi_events, WIFI_CONNECTED_BIT);
    }
}

static void request_page(void *arg)
{
    esp_http_client_config_t config = {
        .url = "https://raw.githubusercontent.com/espressif/esp-idf/master/examples/get-started/blink/main/blink.c",
        .event_handler = handle_http_event,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    if (esp_http_client_perform(client) != ESP_OK)
    {
        ESP_LOGE(TAG, "http request failed");
    }
    esp_http_client_cleanup(client);

    vTaskDelete(NULL);
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



