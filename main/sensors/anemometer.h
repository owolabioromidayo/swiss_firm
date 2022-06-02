#ifndef PROJ_SENSOR_ANEMOMETER_H_
#define PROJ_SENSOR_ANEMOMETER_H_

#include "../config.h"
#include "esp_timer.h"

void measure_windspeed(void *pvParameters);

#endif //PROJ_SENSOR_ANEMOMETER_H_