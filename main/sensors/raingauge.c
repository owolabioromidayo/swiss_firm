#include "./raingauge.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define ESP_INTR_FLAG_DEFAULT 0

static void init_hw(void);
static void rain_int_consumer(void *pvParameters);
static void IRAM_ATTR raingauge_handler(void *arg);

static TickType_t next = 0;
static QueueHandle_t rain_int_queue = NULL;
static int tick_count = 0;

void measure_rainfall(void *pvParameters)
{
    init_hw();
    rain_int_queue = xQueueCreate(10, sizeof(int));
    xTaskCreatePinnedToCore(rain_int_consumer, "rain_interrupt_consumer", 2048, NULL, 10-1, NULL, 0);
    
    int time_period = 20 ; //read for 20s
    int64_t time_period_micros = time_period * 1000000; //microseconds
    double tick_multiplier = 3600 / (double)(time_period - 2); //account for time inbetween function calls 

    vTaskDelay((TickType_t)(time_period*1000 / portTICK_PERIOD_MS)); //wait for 20s to pick up any rain rick interrupts

    float ret = 0.2794*(tick_count+1) * tick_multiplier ; // estimate hourly precipitation from time period (mm/hour)

    *(float *)pvParameters = ret; //save into passred rainfall
    tick_count = 0; //reset tickCount
    gpio_isr_handler_remove(GPIO_RAINGAUGE);

    vTaskDelete(NULL);
}

static void IRAM_ATTR raingauge_handler(void *arg)
{
    TickType_t now = xTaskGetTickCountFromISR();
    if (next > now)
    {
        return;
    }
    next = now + 500/portTICK_PERIOD_MS;
    int _interrupt = (int)(arg);
    xQueueSendToBackFromISR(rain_int_queue, &_interrupt, NULL);
}

static void rain_int_consumer(void *pvParameters)
{
    uint8_t _interrupt;
    int64_t start = esp_timer_get_time();
    int time_period = 18 ; //read for 18s
    int64_t time_period_micros = time_period * 1000000; //microseconds

    while(abs(esp_timer_get_time() - start) < time_period_micros)
    {
        if (xQueueReceive(rain_int_queue, &_interrupt, portMAX_DELAY))
        {
            tick_count += _interrupt;
            printf("RECEIVED INTERRUPT, TICK COUNT : %i ; LEVEL: %i \n", tick_count, _interrupt);
        }
    }

    vTaskDelete(NULL);
}


static void init_hw(void)
{
    gpio_config_t io_conf;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_RAINGAUGE_SEL;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO_RAINGAUGE, raingauge_handler, NULL);
}


