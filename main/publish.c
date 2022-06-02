#include "publish.h"


static sensor_values_t gather_data(void);

int send_post()
{ 
    // part of wifi service?
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
    sensor_values_t v = gather_data();
    wifi_err_t ok  = post_data(v);
    //create task here, wait for 2 min

    return 1;
}


//collect all sensor data and pack as struct to be sent over network
static sensor_values_t gather_data(void)
{
    sensor_values_t v;

    v.ext_temp = get_ext_temp();
    bme680_values_float_t bme_vals = bme680_get_values();
    v.temp  = bme_vals.temperature;
    v.humidity = bme_vals.humidity;
    v.baro_pressure = bme_vals.pressure;
    v.gas_resistance = bme_vals.gas_resistance;
    v.rainfall = v.battery_percentage = v.solar_irradiance = v.wind_direction = v.wind_speed = 0;
    
    return v;

}