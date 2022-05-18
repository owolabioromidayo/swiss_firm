#ifndef PROJ_SENSOR_ANEMOMETER_H_
#define PROJ_SENSOR_ANEMOMETER_H_

#include "driver/gpio.h"
#include "../config.h"
#include "esp_timer.h"

float measure_windspeed(void);

#endif //PROJ_SENSOR_ANEMOMETER_H_