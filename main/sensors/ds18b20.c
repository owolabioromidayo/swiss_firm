#include "./ds18b20.h"
#include <stdio.h>

#define DSB_SAMPLE_CNT 10

static ds18x20_addr_t addrs[MAX_SENSORS];
static int sensor_count = 0;
static float temps[MAX_SENSORS];

static void init_hw(void);

float get_ext_temp(void)
{
    init_hw();

    float temp_acc = 0;

    for(int i=0; i < DSB_SAMPLE_CNT; ++i) // read multiple to stabilize readings
    {
        ds18x20_measure_and_read_multi((gpio_num_t)GPIO_DSB, addrs, sensor_count, temps);
        for (int i = 0; i < sensor_count; i++)
        {
            printf("sensor-id: %08x temp: %fC\n", (uint32_t)addrs[i], temps[i]);
            temp_acc += temps[i];
        }
    }

    return temp_acc / DSB_SAMPLE_CNT;
}

static void init_hw(void)
{
    size_t found = 0;
    while (sensor_count == 0)
    {
        sensor_count = ds18x20_scan_devices((gpio_num_t)GPIO_DSB, addrs, MAX_SENSORS, &found);
        printf("ONE WIRE SENSOR COUNT: %d \n", found);
        sensor_count = found;
    }
    if (sensor_count > MAX_SENSORS)
    {
        sensor_count = MAX_SENSORS;
    }
}


//also we should warm it up before hand
//eventually we will want to get the median of 3 readings for better accuracy