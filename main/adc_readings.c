#include "adc_readings.h"

#define TAG "adc_readings"
#define SAMPLE_CNT 20

// MEASURING RANGES FOR ADC_ATTEN_DB_11  150 mV ~ 2450 mV
// Also taking into account max input voltage 2100mV and assuming battery is always connected
// #define BATTERY_CUTOFF_LOW 0 
#define BATTERY_CUTOFF_LOW 1000
#define BATTERY_CUTOFF_HIGH 2250 
// #define BATTERY_CUTOFF_HIGH 4000 
#define BATTERY_MULT_FACTOR 1.00

static esp_adc_cal_characteristics_t battery_adc_chars;
static const adc1_channel_t adc_battery_channel = ADC_BATTERY_PIN;

static bool init_adc(void)
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
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_10, 0, &battery_adc_chars);
        ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_10));
        ESP_ERROR_CHECK(adc1_config_channel_atten(adc_battery_channel, ADC_ATTEN_DB_11));
    }
    else 
        ESP_LOGE(TAG, "Invalid arg");
    
    return cali_enable;
}


int get_battery_percentage(void) 
{
    bool cali_enable = init_adc();

    uint32_t voltage_readings = 0;
    int curr_voltage_reading_raw = 0;
    uint32_t curr_voltage_reading = 0;
    
    for (int i = 0; i < SAMPLE_CNT*4; ++i)
        adc1_get_raw(adc_battery_channel); //take garbage readings, warm up ADC

    if(!cali_enable)
        return -1;

    for (int i = 0; i < SAMPLE_CNT; ++i)
    {
        curr_voltage_reading_raw = adc1_get_raw(adc_battery_channel);
        curr_voltage_reading =  esp_adc_cal_raw_to_voltage(curr_voltage_reading_raw, &battery_adc_chars);
        
        if ( BATTERY_CUTOFF_LOW <= curr_voltage_reading && curr_voltage_reading<= BATTERY_CUTOFF_HIGH)
            voltage_readings += curr_voltage_reading;
        else 
            i--;    
        vTaskDelay((TickType_t)(0.001*1000 / portTICK_PERIOD_MS));
    }

    float avg_voltage = (BATTERY_MULT_FACTOR * voltage_readings) / (SAMPLE_CNT*1000);
    float percentage = (avg_voltage * 100) / 2.1 ; 
    return (int)percentage;
}
