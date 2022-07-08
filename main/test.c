#include <stdio.h>
#include "test.h"

//Most of these tests do not have print statements themselves as it is already contained in the code.
//You should still see them from the terminal


void test_ds18b20(void)
{
    get_ext_temp();
    printf("DS18B20 Test Complete!\n");
}

void test_bme680(void)
{
    bme680_get_values();
    printf("BME680 Test Complete!\n");
}
    

void test_si1145(void)
{
    int uv = si1145_get_uv();
    printf("UV : %d\n", uv);
    printf("SI1145 Test Complete!\n");
}

void test_battery_adc(void)
{
    bat = get_battery_percentage();
    printf("Batery Percentage: %d\n", bat );
    printf("Battery Percentage Test Complete!\n");

}

void test_wind_vane(void)
{
    bool vane_cali_enable = init_wind_vane_adc();
    if (vane_cali_enable)
    {
        wvane_ret a  = getWindDirection();
        vTaskDelay((TickType_t)(1*1000 / portTICK_PERIOD_MS));
        char* winw  = wind_cardinal_map[a.pos];
        v.wind_direction = winw;
        printf("Analog: %f, Direction: %d \n", a.analog, a.pos);
    }

    printf("Wind Vane Test Complete! \n");

}


void test_all(void)
{
    test_bme680();
    test_ds18b20();
    test_si1145();
    test_wind_vane();
    test_battery_adc(); 

    printf("All tests done!\n");
}