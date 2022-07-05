#ifndef PROJ_TYPES_H_
#define PROJ_TYPES_H_

typedef struct 
{
    float internal_temp;
    float ext_temp;
    float humidity;
    int gas_resistance;
    float baro_pressure;
    int battery_percentage;
    float wind_speed;
    char* wind_direction;
    float precipitation_mmhr;
    int uv;
} sensor_values_t;

typedef struct 
{
    float analog;
    int pos;
}wvane_ret;

#endif //PROJ_TYPES_H_