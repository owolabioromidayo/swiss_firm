// create a config.h from this (change all the values to yours)
#ifndef PROJ_MAIN_CONF_H_
#define PROJ_MAIN_CONF_H_

#include "driver/adc.h"
#include "driver/gpio.h"

#define TEST_MODE 1
#define WIFI_SSID "ssid"
#define WIFI_SSID_ESP "esp32_ssid"
#define WIFI_PASS "pass"
#define WIFI_PWD "esp_pass"
#define HOSTNAME "http://www.google.com"

// typedef enum {
//     ADC1_CHANNEL_0 = 0, /*!< ADC1 channel 0 is GPIO36 */
//     ADC1_CHANNEL_1,     /*!< ADC1 channel 1 is GPIO37 */
//     ADC1_CHANNEL_2,     /*!< ADC1 channel 2 is GPIO38 */
//     ADC1_CHANNEL_3,     /*!< ADC1 channel 3 is GPIO39 */
//     ADC1_CHANNEL_4,     /*!< ADC1 channel 4 is GPIO32 */
//     ADC1_CHANNEL_5,     /*!< ADC1 channel 5 is GPIO33 */
//     ADC1_CHANNEL_6,     /*!< ADC1 channel 6 is GPIO34 */
//     ADC1_CHANNEL_7,     /*!< ADC1 channel 7 is GPIO35 */
//     ADC1_CHANNEL_MAX,
// } adc1_channel_t;
//use this for reference when deciding on your ADC pins. 

#define ADC_WIND_VANE_PIN ADC_CHANNEL_7
#define ADC_BATTERY_PIN ADC_CHANNEL_3

#define GPIO_ANEMOMETER GPIO_NUM_19
#define GPIO_ANEMOMETER_SEL (1ULL << GPIO_ANEMOMETER)

#define GPIO_RAINGAUGE GPIO_NUM_11
#define GPIO_RAINGAUGE_SEL (1ULL << GPIO_RAINGAUGE)

#define GPIO_DSB GPIO_NUM_8

#define GPIO_SDA GPIO_NUM_21
#define GPIO_SCL GPIO_NUM_22

#endif //PROJ_MAIN_CONF_H_

