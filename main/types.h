#ifndef PROJ_TYPES_H_
#define PROJ_TYPES_H_

typedef struct sensor_values_t
{
    float temp;
    float ext_temp;
    float humidity;
    float gas_resistance;
    float baro_pressure;
    float battery_percentage;
    float solar_irradiance;
    float wind_speed;
    float wind_direction;
    float rainfall;
} sensor_values_t;

#endif //PROJ_TYPES_H_