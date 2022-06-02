#ifndef PROJ_WIFI_H_
#define PROJ_WIFI_H_

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
#include "types.h"

typedef int wifi_err_t;

#define WIFI_ERR 0
#define WIFI_OK 1


void init_wifi(void *pvParameters);
wifi_err_t post_data(sensor_values_t v);


#endif //PROJ_WIFI_H_