#ifndef PROJ_SENSOR_WIND_VANE_H_
#define PROJ_SENSOR_WIND_VANE_H_

#include "../config.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"
#include "esp_log.h"

void  getWindDirection(char *dest);

#endif // PROJ_SENSOR_WIND_VANE_H_