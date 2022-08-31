
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
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"

#include "utils.h"
#include "types.h"
#include "test.h"

#include "sensors/wind_vane.h"
#include "sensors/anemometer.h"
#include "soc/rtc_wdt.h"



#define TAG "weather_station"
#define HOUR_MICROS 60*60*1000000
#define HOUR_SECS 1*60
#define SEC_MICROS 1000000l
#define SLEEP_TIME_MIN 45
#define WIFI_WAIT_PERIOD_MIN 3

static uint64_t SLEEP_TIME_MICROS = (uint64_t)SLEEP_TIME_MIN * (uint64_t)SEC_MICROS * 60;
static RTC_DATA_ATTR int rainTicks = 0; 
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
    if (TEST_MODE){
        test_all();
        return;
    }
    
    if(elapsed_time == 0)
        time(&elapsed_time);
    

    rtc_wdt_protect_off();
    rtc_wdt_disable();

    while(1){


        wakeup_reason();
        if (wifi_enable) 
        {

            //get readings before wifi initialization to not corrupt adc2 readings
            float windspeed = 0;
            bool vane_cali_enable = init_wind_vane_adc();
            init_anemometer_hw();

            sensor_values_t v = gather_data();
            v.precipitation_mmhr = hourlyTicks;

            xTaskCreatePinnedToCore(measure_windspeed, "measure_windspeed", 2000, (void *)&windspeed, 10, NULL, 0);
            if (vane_cali_enable)
            {
                wvane_ret a  = getWindDirection();
                vTaskDelay((TickType_t)(3*1000 / portTICK_PERIOD_MS));
                char* winw  = wind_cardinal_map[a.pos];
                v.wind_direction = winw;
                printf("Analog: %f, Direction: %d \n", a.analog, a.pos);
            }

            //wait for wind speed procedure
            wifi_manager_start();
            wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);
            vTaskDelay((TickType_t)(25*1000 / portTICK_PERIOD_MS)); //wait for wifi conn
            printf("Windspeed: %f\n", windspeed);
            v.wind_speed = windspeed;

            if(wifi_connected)
            {
                ESP_LOGI(TAG, "Posting now. \n");
                post_data(v);
            }
            else{
                //longer delayed wait period 
                ESP_LOGI(TAG, "Entering wifi wait period now. \n");
                vTaskDelay((TickType_t)(WIFI_WAIT_PERIOD_MIN*60*1000 / portTICK_PERIOD_MS));
                if(wifi_connected)
                    post_data(v);
                else
                    ESP_LOGI(TAG, "Failed to connect to WiFi. Going to sleep. \n");
            }
        }
        esp_sleep_enable_timer_wakeup(SLEEP_TIME_MICROS);
        esp_sleep_enable_ext0_wakeup(GPIO_RAINGAUGE, 0);
        ESP_LOGI(TAG, "Entering DEEP SLEEP \n");
        esp_deep_sleep_start();
    }
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
    vTaskDelete(NULL);
}
