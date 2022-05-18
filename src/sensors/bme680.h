#ifndef PROJ_BME_H_
#define PROJ_BME_H_

#include <bme680.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <string.h>
#include "../config.h"

#define PORT 0

bme680_values_float_t bme680_get_values(void);

#endif // PROJ_BME_H_
