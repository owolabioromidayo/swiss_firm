#include "adc_readings.h"
#include <stdio.h>

#define SAMPLE_CNT 100
static const adc1_channel_t adc_battery_channel = ADC_BATTERY_PIN;
static const adc1_channel_t adc_solar_channel = ADC_SOLAR_PANEL_PIN;
// static const float MAX_BATTERY_VOLTAGE_READING = (VOLTAGE_BATTERY_MAX * BATTERY_R2) / (BATTERY_R1 + BATTERY_R2); //voltage divider


static void init_battery_adc(){
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(adc_battery_channel, ADC_ATTEN_DB_11);
}

static void init_solar_adc(){
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(adc_solar_channel, ADC_ATTEN_DB_11);
}


int get_battery_percentage(void){
    init_battery_adc();

    float voltage_readings = 0;
    int32_t curr_voltage_reading_raw = 0;
    float curr_voltage_reading = 0;
    float MAX_ADC_VAL = 1024.0;
    int32_t CUTOFF = 400; //150mV 
    for (int i = 0; i < SAMPLE_CNT; ++i)
    
        adc1_get_raw(adc_battery_channel); //take garbage readings;

    for (int i = 0; i < SAMPLE_CNT; ++i)
    {
        curr_voltage_reading_raw = adc1_get_raw(adc_battery_channel);
        if (curr_voltage_reading_raw > CUTOFF && curr_voltage_reading_raw < 1000 )
            curr_voltage_reading = 2.45*( (float)curr_voltage_reading_raw / MAX_ADC_VAL);
        printf("RAW: %d, RE: %f MAX: %f DIV: %f\n", curr_voltage_reading_raw, curr_voltage_reading, MAX_ADC_VAL, curr_voltage_reading);
        voltage_readings += curr_voltage_reading;
        curr_voltage_reading = 0;
        printf("Acc: %f \n", voltage_readings);
    }

    float avg_voltage = voltage_readings / SAMPLE_CNT;
    float percentage = (avg_voltage / 2.1) * 100; 
    printf("Avg voltage: %f, Percentage %f \n", avg_voltage, percentage);
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