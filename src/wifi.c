#include "storage.h"
#include "wifi.h"

//ensure you change this before deployment


// #define WIFI_SSID "esp32_ap1"
#define WIFI_PWD "reconnect"

#define WIFI_SSID "NokiaG20"
#define WIFI_PASS "nicetryu"

#define WIFI_CHANNEL 11
#define MAX_CONN_CNT 1
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define MAX_RETRY 10
#define WIFI_OK 0
#define WIFI_ERR -1
#define W_SIZE 20 //wifi ssid/pass max len
// #define NO_CONFIG_WIFI 1
// #define CONFIG_WIFI 0

static const char *TAG = "wifi_service";
static const char *HTML_FORM = "<html><form action=\"/\" method=\"post\">"
                               "<label for=\"ssid\">Local SSID:</label><br>"
                               "<input type=\"text\" id=\"ssid\" name=\"ssid\"><br>"
                               "<label for=\"pwd\">Password:</label><br>"
                               "<input type=\"text\" id=\"pwd\" name=\"pwd\"><br>"
                               "<input type=\"submit\" value=\"Submit\">"
                               "</form></html>";

//wifi details
static void ensure_wifi_details();
static int test_wifi();
static int test_wifi_from_webserver(char* ssid, char* pass);
static void run_ap_till_working_details();
static void start_form_webserver(void);
static esp_err_t handle_http_form_get(httpd_req_t *req);
static esp_err_t handle_http_form_post(httpd_req_t *req);

//AP protos
// static void handle_wifi_events(void *, esp_event_base_t, int32_t, void *);

//STA protos
// static void request_page(void *);
static esp_err_t handle_http_event(esp_http_client_event_t *);
// static void handle_wifi_connection(void *, esp_event_base_t, int32_t, void *);


wifi_err_t init_wifi(void){
    if (nvs_flash_init() != ESP_OK){
        nvs_flash_erase();
        nvs_flash_init();
        printf("NVS CLEARED\n");
    }else{
        printf("NVS_ERR\n");
    }

    esp_netif_init(); //Initialize the underlying TCP/IP stack.
    // ensure_wifi_details();

    //temp use known AP
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };

    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
    esp_wifi_connect();

    esp_http_client_config_t config = {
    .url = "http://www.google.com",
    .event_handler = handle_http_event,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    if (esp_http_client_perform(client) != ESP_OK)
    {
        ESP_LOGE(TAG, "http request failed");
    }

    esp_http_client_cleanup(client);
    
}


static void ensure_wifi_details(){
    //ensure we have a working connection before progressing

    //attempt sta connection with details
    char *wifi_ssid = "";
    char *wifi_pass = "";

    int load_err;
    load_err = get_string_value_nvs("WIFI_SSID", wifi_ssid, W_SIZE);
    load_err = get_string_value_nvs("WIFI_PASS", wifi_pass, W_SIZE);

    if (load_err == STORAGE_OK){
        //create wifi sta
        //there are no connections to be made here
        //attempt a connection, if it fails we AP
        int test_err = test_wifi(wifi_ssid, wifi_pass);
        if (test_err == WIFI_OK){
            return;
        }
    }
    //create wifi ap to ask for new details
    //keep it on till it works, then save values and shut off (no max retries)
    run_ap_till_working_details();
    
}


static int test_wifi(char* ssid, char* pass){
    //create a simple http request to ensure internet connectivity

    //create station endpoint
    wifi_config_t wifi_config = {
        .sta = {
            // .ssid = *ssid,
            // .password = *pass,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };

    strcpy((char *)wifi_config.sta.ssid, ssid);
    strcpy((char *)wifi_config.sta.password, pass);

    int status;

    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();

    esp_http_client_config_t config = {
        .url = "https://www.google.com",
        .event_handler = handle_http_event,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (esp_http_client_perform(client) != ESP_OK)
    {
        ESP_LOGE(TAG, "http request failed");
        status = WIFI_ERR;
    }else{
        status = WIFI_OK;
    }

    esp_http_client_cleanup(client);

    return status;
}

static int test_wifi_from_webserver(char* ssid, char* pass){
    //create a simple http request to ensure internet connectivity

    //create station endpoint
    wifi_config_t wifi_config = {
        .sta = {
            // .ssid = ssid,
            // .password = pass,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };

    strcpy((char *)wifi_config.sta.ssid, ssid);
    strcpy((char *)wifi_config.sta.password, pass);

    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();

    esp_http_client_config_t config = {
        .url = "https://www.google.com",
        .event_handler = handle_http_event,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (esp_http_client_perform(client) != ESP_OK)
    {
        ESP_LOGE(TAG, "http request failed");
        esp_http_client_cleanup(client);
        run_ap_till_working_details();
        return ESP_OK;

    }else{

            int load_err;
            load_err = set_string_value_nvs("WIFI_SSID", ssid);
            load_err = set_string_value_nvs("WIFI_PASS", pass);

            if (load_err == STORAGE_OK){
                return ESP_OK ;
            }else{
                load_err = set_string_value_nvs("WIFI_SSID", ssid);
                load_err = set_string_value_nvs("WIFI_PASS", pass);
                return ESP_OK;
            }
    }

    esp_http_client_cleanup(client);
    return ESP_FAIL;
}



static void run_ap_till_working_details(){
   
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

    start_form_webserver();
}


//STA functions
// static void handle_wifi_events(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
// {
//     if (event_id == WIFI_EVENT_AP_STACONNECTED)
//     {
//         ESP_LOGI(TAG, "a station connected");
//     }
// }


static esp_err_t handle_http_form_get(httpd_req_t *req){
    //return html form to enter new wifi details
    return httpd_resp_send(req, HTML_FORM, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t handle_http_form_post(httpd_req_t *req){
    //on wifi form submission
    char content[100];
    if (httpd_req_recv(req, content, req->content_len) <= 0){ //form not filled
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "%.*s", req->content_len, content);
    //here we need to separate the new values (but for now)
    // char wifi_ssid[W_SIZE] = "NokiaG20\0";
    // char wifi_pass[W_SIZE] = "nicetry\0";

    //here we need to split the string

    // if (test_wifi(wifi_ssid, wifi_pass) == WIFI_OK){
    //     int set_err;
    //     set_err = set_string_value_nvs("WIFI_SSID", wifi_ssid);
    //     set_err = set_string_value_nvs("WIFI_PASS", wifi_pass);

    //     if (set_err == STORAGE_ERR){
    //         return httpd_resp_send(req, 
    //         "Details were correct but storage failed. Please retry.", HTTPD_RESP_USE_STRLEN);
    //     }
        
    //     resp_sent = httpd_resp_send(req, "Successful!", HTTPD_RESP_USE_STRLEN);
    //     // send a signal to end server

    //     return resp_sent;

    // }

    return httpd_resp_send(req, "Received.", HTTPD_RESP_USE_STRLEN);

    // char *ssid = "NokiaG20";
    // char *pass = "nicetryu";
    // return test_wifi_from_webserver(ssid, pass);

}


static void start_form_webserver(void){
    //collect wifi details until we have a working connection.
    httpd_uri_t uri_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = handle_http_form_get,
        .user_ctx = NULL};

    httpd_uri_t uri_post = {
        .uri = "/",
        .method = HTTP_POST,
        .handler = handle_http_form_post,
        .user_ctx = NULL};

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK)
    {
        printf("HTTP CONFIG SET!\n");
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_post);
        printf("HTTP HANDLERS SET!\n");
    }else{
        printf("Failed to start HTTP server! \n");
    }
}


//AP functions
// static void handle_wifi_connection(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
// {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
//     {
//         esp_wifi_connect();
//     }
//     else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
//     {
//         if (retry_cnt++ < MAX_RETRY)
//         {
//             esp_wifi_connect();
//             ESP_LOGI(TAG, "wifi connect retry: %d", retry_cnt);
//         }
//         else
//         {
//             xEventGroupSetBits(wifi_events, WIFI_FAIL_BIT);
//         }
//     }
//     else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
//     {
//         ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
//         ESP_LOGI(TAG, "ip: %d.%d.%d.%d", IP2STR(&event->ip_info.ip));
//         retry_cnt = 0;
//         xEventGroupSetBits(wifi_events, WIFI_CONNECTED_BIT);
//     }
// }

// static void request_page(void *arg)
// {
//     esp_http_client_config_t config = {
//         .url = "https://raw.githubusercontent.com/espressif/esp-idf/master/examples/get-started/blink/main/blink.c",
//         .event_handler = handle_http_event,
//     };
//     esp_http_client_handle_t client = esp_http_client_init(&config);

//     if (esp_http_client_perform(client) != ESP_OK)
//     {
//         ESP_LOGE(TAG, "http request failed");
//     }
//     esp_http_client_cleanup(client);

//     vTaskDelete(NULL);
// }

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



