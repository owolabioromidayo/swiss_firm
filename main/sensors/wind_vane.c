#include "./wind_vane.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TAG "wind_vane"
#define SAMPLE_CNT 20

static const adc1_channel_t adc_wind_vane_channel = ADC_WIND_VANE_PIN;
static esp_adc_cal_characteristics_t wind_vane_adc_chars;

static bool init_wind_vane_adc(void);

void getWindDirection(char* dest)
{
    bool cali_enable = init_wind_vane_adc();

    int analog_voltage_map[16] = { 210, 270, 300, 410, 600, 790, 930, 1310, 1490, 1930, 2030, 2260, 2530, 
                                2670, 2860, 3050 };

    char* wind_cardinal_map[16] = { "ESE", "ENE", "E", "SSE", "SE", "SSW", "S", "NNE", "NE", "WSW","SW", "NNW", "N"
                                "WNW","NW", "W"};

    for (int i = 0; i < SAMPLE_CNT*4; ++i)
        adc1_get_raw(adc_wind_vane_channel); //take garbage readings, warm up ADC


    if (!cali_enable)
        return "cant read";

    int adc_vin = 0;
    int adc_raw = adc1_get_raw(adc_wind_vane_channel);
    for (int i = 0; i < SAMPLE_CNT; ++i)
    {
        adc_raw = adc1_get_raw(adc_wind_vane_channel); 
        adc_vin += esp_adc_cal_raw_to_voltage(adc_raw, &wind_vane_adc_chars);
    }
    float avg_vin = (float)(adc_vin) / SAMPLE_CNT;

    for (int i = 0; i < 16; i++)
    {
        if (avg_vin < analog_voltage_map[i])
        {
            strcpy(dest, wind_cardinal_map[i]);
            break;
        }
    }
}
    // printf("Analog value: %f Wind direction:, %s  \n", avg_vin, wind_cardinal_direction);

static bool init_wind_vane_adc(void)
{
    esp_err_t ret;
    bool cali_enable = false;

    ret = esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF);
    if (ret == ESP_ERR_NOT_SUPPORTED) 
        ESP_LOGW(TAG, "Calibration scheme not supported, skip software calibration");
    else if (ret == ESP_ERR_INVALID_VERSION) 
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    else if (ret == ESP_OK) 
    {
        cali_enable = true;
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_10, 0, &wind_vane_adc_chars);
    } 
    else 
        ESP_LOGE(TAG, "Invalid arg");

    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_10));
    ESP_ERROR_CHECK(adc1_config_channel_atten(adc_wind_vane_channel, ADC_ATTEN_DB_11));

    return cali_enable;
}