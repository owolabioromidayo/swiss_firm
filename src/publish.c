#include "./publish.h"
#include <stdio.h>


struct Values{
    float temp;
    float ext_temp;
    float humidity;
    float air_quality;
    float baro_pressure;
    float battery_percentage;
    float solar_irradiance;
    float wind_speed;
    float wind_direction;
    float rainfall;
};


static struct Values gather_data(void);

int send_post(){ // part of wifi service?
    // esp_http_client_config_t config = {
    //     .url = "https://raw.githubusercontent.com/espressif/esp-idf/master/examples/get-started/blink/main/blink.c",
    //     .event_handler = handle_http_event,
    // };
    // esp_http_client_handle_t client = esp_http_client_init(&config);

    // if (esp_http_client_perform(client) != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "http request failed");
    // }
    // esp_http_client_cleanup(client);

    //conv data to json
    //publish data using wifi service -> this will handle the wifi issues
    //return err or ok

    //for now...
    struct Values v = gather_data();
    printf("%f TEMPERATURE  \n", v.temp);

    bme680_values_float_t bme_vals = bme680_get_values();
    v.humidity  = bme_vals.humidity;
    v.baro_pressure  = bme_vals.pressure;
    v.temp  = bme_vals.temperature;
    v.air_quality = bme_vals.gas_resistance;
    



    return 1;
}


static struct Values gather_data(void){// collect all sensor data and pack as struct
    struct Values v;

    // v.temp = get_ext_temp(); NEEDS TO BE FIXED
    v.temp = 33.6;

    return v;

}