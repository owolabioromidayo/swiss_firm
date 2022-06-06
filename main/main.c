#include "wifi.h"
#include "wifi_manager.h"
// #include "storage.h"
#include "publish.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include <stdio.h>
#include "sensors/raingauge.h"

// static void cb_connection_ok(void *pvParameters);

void app_main(void)
{
    printf("IN MAIN FUNC \n");
    float rainfall = 0.0;
    xTaskCreatePinnedToCore(measure_rainfall, "measure_rainfall", 4096, (void *)&rainfall, 10, NULL, 1);
    vTaskDelay((TickType_t)(23*1000 / portTICK_PERIOD_MS));
    printf("Rainfall: %f\n", rainfall);

    // wifi_manager_start();
    // wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);
    
    return;
}


// static void cb_connection_ok(void *pvParameters)
// {
//     sensor_values_t v;
//     v.temp = v.ext_temp = v.baro_pressure = v.humidity = v.gas_resistance = v.wind_direction = v.wind_speed = v.battery_percentage = v.solar_irradiance = v.rainfall = 1.5;
//     post_data(v);
// }