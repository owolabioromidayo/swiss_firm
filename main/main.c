// #include "wifi.h"
// #include "wifi_manager.h"
// #include "storage.h"
#include "publish.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include <stdio.h>
#include "sensors/raingauge.h"
#include "adc_readings.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "esp_timer.h"
#include "esp_log.h"


#define TAG "weather_station"
// #define HOUR_MICROS 60*60*1000000
#define HOUR_MICROS 60*1000000
#define HOUR_SECS 1*60
#define SEC_MICROS 1000000l
#define SLEEP_TIME_MIN 2


static RTC_DATA_ATTR int rainTicks = 0; //keep hourly track
static RTC_DATA_ATTR time_t elapsed_time = 0;
static RTC_DATA_ATTR int hourlyTicks = 0; //keep hourly track
static RTC_DATA_ATTR time_t next = 0;
static bool wifi_enable = true;


// static void cb_connection_ok(void *pvParameters);
static void wakeup_reason(void);
static void handle_rain_tick(void);
static void handle_hourly_tick(void);

void app_main(void)
{
    ESP_LOGI(TAG, "IN MAIN FUNC \n");
    if(elapsed_time == 0)
        time(&elapsed_time);
    
    while(1){
        ESP_LOGI(TAG, "IN LOOP");

        wakeup_reason();
        if (wifi_enable)
        {
            //setup everything here again
            // wifi_manager_start();
            // wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);
        }


        // xTaskCreatePinnedToCore(measure_rainfall, "measure_rain  fall", 4096, (void *)&rainfall, 10, NULL, 1);
        // vTaskDelay((TickType_t)(23*1000 / portTICK_PERIOD_MS));
        // int percentage = get_battery_percentage();
        // ESP_LOGI(TAG, "Battery Percentage: %d percent. \n", percentage);

        esp_sleep_enable_timer_wakeup(SLEEP_TIME_MIN*60*SEC_MICROS);
        esp_sleep_enable_ext0_wakeup(GPIO_RAINGAUGE, 0);
        ESP_LOGI(TAG, "Entering DEEP SLEEP \n");
        esp_deep_sleep_start();
    }

    return;
}

static void wakeup_reason(void){
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();
    ESP_LOGI(TAG, "Wakeup reason: %d\n", wakeup_reason);
    switch (wakeup_reason)
    {
    //Rain Tip Gauge
    case ESP_SLEEP_WAKEUP_EXT0 :
        ESP_LOGI(TAG, "Wakeup caused by external signal using RTC_IO\n");
        wifi_enable = false;
        handle_rain_tick();
        ESP_LOGI(TAG, "RainTicks: %d \n", rainTicks);
        break;

    case ESP_SLEEP_WAKEUP_TIMER:
        ESP_LOGI(TAG, "Wakeup caused by timer\n");
        wifi_enable = true;
        break;

    //Initial boot or other default reason
    default :
        ESP_LOGI(TAG, "Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        wifi_enable = true;
        // attach_raingauge_interrupt();
        break;
    }
    handle_hourly_tick();
}

static void handle_hourly_tick(void)
{
    if(time(NULL) - elapsed_time >= HOUR_SECS - 30)
    {
        hourlyTicks = rainTicks;
        rainTicks = 0; 
        elapsed_time = time(NULL);
        ESP_LOGI(TAG, "Hourly Tick Count: %d \n", hourlyTicks);
    }
}

static void handle_rain_tick()
{
    //prevent debouncing
    time_t now = time(NULL);

    if (next > now)
    {
        ESP_LOGD(TAG, "Debouncing\n");
        return;
    }
    next = now + 1;
    rainTicks++;
   
}
// static void cb_connection_ok(void *pvParameters)
// {
//     //here we get all the values to send
//     sensor_values_t v;
//     v.temp = v.ext_temp = v.baro_pressure = v.humidity = v.gas_resistance = v.wind_direction = v.wind_speed = v.battery_percentage = v.solar_irradiance = v.rainfall = 1.5;
//     post_data(v);
// }