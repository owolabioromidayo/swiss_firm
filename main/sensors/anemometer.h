#ifndef PROJ_SENSOR_ANEMOMETER_H_
#define PROJ_SENSOR_ANEMOMETER_H_

#include "../config.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

void init_anemometer_hw(void);
void measure_windspeed(void *pvParameters);

#endif //PROJ_SENSOR_ANEMOMETER_H_