// create a config.h from this (change all the values to yours)
#ifndef PROJ_MAIN_CONF_H_
#define PROJ_MAIN_CONF_H_


#include "driver/adc.h"
#include "driver/gpio.h"

#define WIFI_SSID "ssid"
#define WIFI_SSID_ESP "esp32_ssid"
#define WIFI_PASS "pass"
#define WIFI_PWD "esp_pass"
#define HOSTNAME "http://www.google.com"

#define ADC_WIND_VANE_PIN ADC_CHANNEL_7
#define ADC_BATTERY_PIN ADC_CHANNEL_3
#define ADC_SOLAR_PANEL_PIN ADC_CHANNEL_5

#define GPIO_ANEMOMETER GPIO_NUM_19
#define GPIO_ANEMOMETER_SEL (1ULL << GPIO_ANEMOMETER)

#define GPIO_RAINGAUGE GPIO_NUM_11
#define GPIO_RAINGAUGE_SEL (1ULL << GPIO_RAINGAUGE)

#define GPIO_DSB GPIO_NUM_8

#define GPIO_BME680_SDA GPIO_NUM_9
#define GPIO_BME680_SCL GPIO_NUM_10

#define SOLAR_PANEL_AREA_M2 0.076 
#define RES_SOLAR_PANEL  10000
#define VOLTAGE_SOLAR_PANEL 5.0

#endif //PROJ_MAIN_CONF_H_

