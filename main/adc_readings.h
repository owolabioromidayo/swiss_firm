#ifndef PROJ_ADC_READINGS_H_
#define PROJ_ADC_READINGS_H_

#include "driver/adc.h"
#include "config.h"

// void get_battery_percentage(void *pvParameters);
int get_battery_percentage(void);
double get_solar_irradiance(void);

#endif //PROJ_ADC_READINGS_H_
