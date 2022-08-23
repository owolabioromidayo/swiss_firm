#ifndef PROJ_ADC_READINGS_H_
#define PROJ_ADC_READINGS_H_

#include "driver/adc.h"
#include "config.h"
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

int get_battery_percentage(void);

#endif //PROJ_ADC_READINGS_H_
