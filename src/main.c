// #include "wifi.h"
// #include "storage.h"
// #include "publish.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include <stdio.h>
// #include "sensors/bme680.h"
#include "sensors/raingauge.h"


void app_main(void)
{
    printf("IN MAIN FUNC \n");
    // init_storage();

    // init_wifi();
    // xTaskCreate(init_wifi, "wifi_service", 10 * configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    // if (init_wifi() == WIFI_ERR){
    //     print("Error initializing Wifi");
    //     return;
    // }
    // send_post(62.8);


    float rainfall = 0.0;
    xTaskCreatePinnedToCore(measure_rainfall, "measure_rainfall", 4096, (void *)&rainfall, 10, NULL, 1);
    vTaskDelay((TickType_t)(23*1000 / portTICK_PERIOD_MS));
    printf("Rainfall: %f\n", rainfall);
    
    return;
}
