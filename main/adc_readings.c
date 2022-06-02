#include "adc_readings.h"

#define SAMPLE_CNT 30
static const adc1_channel_t adc_battery_channel = ADC_BATTERY_PIN;
static const adc1_channel_t adc_solar_channel = ADC_SOLAR_PANEL_PIN;
static const float MAX_BATTERY_VOLTAGE_READING = (VOLTAGE_BATTERY_MAX * BATTERY_R2) / (BATTERY_R1 + BATTERY_R2); //voltage divider


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
    for (int i = 0; i < SAMPLE_CNT; ++i)
        adc1_get_raw(adc_battery_channel); //take garbage readings;

    for (int i = 0; i < SAMPLE_CNT; ++i)
        voltage_readings += adc1_get_raw(adc_battery_channel); //real readings now;

    float avg_voltage = voltage_readings / SAMPLE_CNT;
    return (int)((avg_voltage / MAX_BATTERY_VOLTAGE_READING) * 100);


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