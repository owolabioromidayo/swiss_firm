#ifndef PROJ_SENSOR_WIND_VANE_H_
#define PROJ_SENSOR_WIND_VANE_H_

#include "../config.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "../types.h"

bool init_wind_vane_adc(void);
wvane_ret getWindDirection(void);

#endif // PROJ_SENSOR_WIND_VANE_H_