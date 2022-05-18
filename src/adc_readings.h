#ifndef PROJ_ADC_READINGS_H_
#define PROJ_ADC_READINGS_H_

#include "driver/adc.h"
#include "./config.h"

int get_battery_percentage(void);
double get_solar_irradiance(void);

#endif PROJ_ADC_READINGS_H_
