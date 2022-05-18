#include "./anemometer.h"
// #define ESP_INTR_FLAG_DEFAULT 0

static void init_hw(void);

float measure_windspeed(void)
{
    init_hw();
    int64_t start = esp_timer_get_time();
    int time_period = 20 ; //read for 20s
    int64_t time_period_micros = time_period * 1000000; //microseconds
    int tick_count = 0;
    
    while(abs(esp_timer_get_time() - start) < time_period_micros)
    {
        tick_count += gpio_get_level(GPIO_ANEMOMETER); //read from gpio
    }

    double ticks_per_second = tick_count / (double)time_period;
    double wind_speed_kmh =  ticks_per_second * 2.4 ; 
    return wind_speed_kmh / 3.6; //wind speed in m/s
}

static void init_hw(void)
{
    gpio_config_t io_conf;

    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_ANEMOMETER_SEL;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pull_down_en = 0; //confirm this
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    // gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    // gpio_isr_handler_add(GPIO_RAINGAUGE, rainggauge_handler, NULL);
}