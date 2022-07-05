#include "wifi.h"
#include "wifi_manager.h"
// #include "storage.h"
#include "publish.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "freertos/event_groups.h"
#include <stdio.h>
#include "adc_readings.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "esp_timer.h"
#include "esp_log.h"

#include "types.h"

#include "sensors/wind_vane.h"
#include "sensors/bme680.h"
#include "sensors/anemometer.h"
#include "sensors/ds18b20.h"

#define TAG "weather_station"
// #define HOUR_MICROS 60*60*1000000
#define HOUR_MICROS 60*1000000
#define HOUR_SECS 1*60
#define SEC_MICROS 1000000l
#define SLEEP_TIME_MIN 2
#define WIFI_WAIT_PERIOD_MIN 5

static RTC_DATA_ATTR int rainTicks = 0; //keep hourly track
static RTC_DATA_ATTR time_t elapsed_time = 0;
static RTC_DATA_ATTR int hourlyTicks = 0; //keep hourly track
static RTC_DATA_ATTR time_t next = 0;
static bool wifi_enable = true;
static bool wifi_connected = false;

char* wind_cardinal_map[16] = { "ESE", "ENE", "E", "SSE", "SE", "SSW", "S", "NNE", "NE", "WSW","SW", "NNW", "N"
                            "WNW","NW", "W"};

static void cb_connection_ok(void *pvParameters);
static void wakeup_reason(void);
static void handle_rain_tick(void);
static void handle_hourly_tick(void);


void app_main(void)
{
    if(elapsed_time == 0)
        time(&elapsed_time);
    
    while(1){

        wakeup_reason();
        if (wifi_enable) // this means it is not just an interrupt from the rain gauge 
        {
            wifi_manager_start();
            wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);

            float windspeed = 0;
            bool vane_cali_enable = init_wind_vane_adc();
            init_anemometer_hw();

            sensor_values_t v = gather_data();
            xTaskCreatePinnedToCore(measure_windspeed, "measure_windspeed", 8000, (void *)&windspeed, 10, NULL, 0);
            if (vane_cali_enable)
            {
                wvane_ret a  = getWindDirection();
                vTaskDelay((TickType_t)(1*1000 / portTICK_PERIOD_MS));
                char* winw  = wind_cardinal_map[a.pos];
                v.wind_direction = winw;
                printf("Analog: %f, Direction: %d \n", a.analog, a.pos);
            }

            //wait for wifi connection + wind speed procedure
            vTaskDelay((TickType_t)(1*60*1000 / portTICK_PERIOD_MS));


            if(wifi_connected)
                post_data(v);
             else{
                //longer delayed wait period 
                vTaskDelay((TickType_t)(WIFI_WAIT_PERIOD_MIN*60*1000 / portTICK_PERIOD_MS));
                if(wifi_connected)
                    post_data(v);
                else
                    ESP_LOGI(TAG, "Failed to connect to WiFi. Going to sleep. \n");
            }
        }

        esp_sleep_enable_timer_wakeup(SLEEP_TIME_MIN*60*SEC_MICROS);
        esp_sleep_enable_ext0_wakeup(GPIO_RAINGAUGE, 0);
        esp_sleep_enable_ext0_wakeup(GPIO_ANEMOMETER, 0);

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
        ESP_LOGI(TAG, "Wakeup caused by external signal from EXT0/Raingauge\n");
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
        break;
    }

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
   
    handle_hourly_tick();
}

static void cb_connection_ok(void *pvParameters)
{
    wifi_connected = true;
}