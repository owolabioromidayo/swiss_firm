#include "publish.h"

//collect all sensor data and pack as struct to be sent over network
sensor_values_t gather_data(void)
{
    sensor_values_t v;

    v.ext_temp = get_ext_temp();
    bme680_values_float_t bme_vals = bme680_get_values();
    v.internal_temp  = bme_vals.temperature;
    v.humidity = bme_vals.humidity;
    v.baro_pressure = bme_vals.pressure;
    v.gas_resistance = bme_vals.gas_resistance;
    v.precipitation_mmhr = 0; //filled in from main service tick count
    v.battery_percentage = get_battery_percentage();
    v.wind_speed = 0; //measured on main too
    v.uv = get_uv_values();
    
    return v;

}