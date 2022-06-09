#include "adc_readings.h"
#include <stdio.h>
#include <stdlib.h>
#include "esp_adc_cal.h"
#include "esp_log.h"
// #include "freertos/task.h"

#define SAMPLE_CNT 50

// MEASURING RANGES FOR ADC_ATTEN_DB_11  150 mV ~ 2450 mV
// Also taking into account max input voltage 2100mV and assuming battery is always connected
#define BATTERY_CUTOFF_LOW 1000
#define BATTERY_CUTOFF_HIGH 2250 
#define BATTERY_MULT_FACTOR 1.18

#define TAG "adc_readings"

static esp_adc_cal_characteristics_t battery_adc_chars;
static const adc1_channel_t adc_battery_channel = ADC_BATTERY_PIN;
static const adc1_channel_t adc_solar_channel = ADC_SOLAR_PANEL_PIN;

static bool init_battery_adc(){
    esp_err_t ret;
    bool cali_enable = false;

    ret = esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF);
    if (ret == ESP_ERR_NOT_SUPPORTED) {
        ESP_LOGW(TAG, "Calibration scheme not supported, skip software calibration");
    } else if (ret == ESP_ERR_INVALID_VERSION) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else if (ret == ESP_OK) {
        cali_enable = true;
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_10, 0, &battery_adc_chars);
    } else {
        ESP_LOGE(TAG, "Invalid arg");
    }

    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_10));
    ESP_ERROR_CHECK(adc1_config_channel_atten(adc_battery_channel, ADC_ATTEN_DB_11));

    return cali_enable;
}

static void init_solar_adc(){
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(adc_solar_channel, ADC_ATTEN_DB_11);
}


int get_battery_percentage(void) 
{
    bool cali_enable = init_battery_adc();

    uint32_t voltage_readings = 0;
    int curr_voltage_reading_raw = 0;
    uint32_t curr_voltage_reading = 0;

    
    for (int i = 0; i < SAMPLE_CNT*4; ++i)
        adc1_get_raw(adc_battery_channel); //take garbage readings, warm up ADC, sleep


    for (int i = 0; i < SAMPLE_CNT; ++i)
    {
        curr_voltage_reading_raw = adc1_get_raw(adc_battery_channel);
        if (cali_enable)
            curr_voltage_reading =  esp_adc_cal_raw_to_voltage(curr_voltage_reading_raw, &battery_adc_chars);
        
        
        if ( BATTERY_CUTOFF_LOW <= curr_voltage_reading && curr_voltage_reading<= BATTERY_CUTOFF_HIGH)
        {
            printf("RAW: %d, RE: %d \n", curr_voltage_reading_raw, curr_voltage_reading );
            voltage_readings += curr_voltage_reading;
        }
        else 
        {
            i--;    
            continue;
        }
    }

    printf("\n Acc: %d \n", voltage_readings);
    float avg_voltage = (BATTERY_MULT_FACTOR * voltage_readings) / (SAMPLE_CNT*1000);
    float percentage = (avg_voltage * 100) / 2.1 ; 
    printf("Percentage %f \n", percentage);


    return (int)percentage;
}


double get_solar_irradiance(){
    init_solar_adc();

    float voltage_readings = 0;
    for (int i = 0; i < SAMPLE_CNT; ++i)
        adc1_get_raw(adc_battery_channel); //take garbage readings;

    for (int i = 0; i < SAMPLE_CNT; ++i)
        voltage_readings += adc1_get_raw(adc_battery_channel); //real readings now;

    float avg_voltage = voltage_readings / SAMPLE_CNT;
    float current = (VOLTAGE_SOLAR_PANEL - avg_voltage) / RES_SOLAR_PANEL; 
    float power = current * VOLTAGE_SOLAR_PANEL;
    
    return power / SOLAR_PANEL_AREA_M2;
}