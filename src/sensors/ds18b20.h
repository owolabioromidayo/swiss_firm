#ifndef PROJ_SENSOR_DS_H_
#define PROJ_SENSOR_DS_H_

#include <ds18x20.h>
#include "driver/gpio.h"

#define DSB_PIN 21
#define MAX_SENSORS 8

float get_ext_temp(void);


#endif // PROJ_SENSOR_DS_H_
