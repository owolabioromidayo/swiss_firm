#include "./anemometer.h"

#define ESP_INTR_FLAG_DEFAULT 0

static void IRAM_ATTR anemometer_handler(void *arg);

static TickType_t next = 0;
static int tick_count = 0;

void init_anemometer_hw(void)
{
    gpio_config_t io_conf;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_ANEMOMETER_SEL;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pull_down_en = 0; 
    io_conf.pull_up_en = 1;
    
    gpio_config(&io_conf);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO_ANEMOMETER, anemometer_handler, NULL);
}

void measure_windspeed(void *pvParameters)
{
    int time_period = 10; //read for set time
    vTaskDelay((TickType_t)(time_period*1000 / portTICK_PERIOD_MS)); 

    gpio_isr_handler_remove(GPIO_ANEMOMETER);
    printf("Tick Count: %d\n", tick_count);
    float ticks_per_second = (float)(tick_count) / time_period;
    float wind_speed_kmh =  ticks_per_second * 2.4 ; 
    float ret =  wind_speed_kmh / 3.6; //wind speed in m/s

    *(float *)pvParameters = ret; //save into passed wind_speed argument
    tick_count = 0; //reset tickCount

    vTaskDelete(NULL);
}

static void IRAM_ATTR anemometer_handler(void *arg)
{
    TickType_t now = xTaskGetTickCountFromISR();
    if (next > now) // delay/debouncing
        return;
    next = now + 10/portTICK_PERIOD_MS;
    tick_count += 1;
}

