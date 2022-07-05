#include "./si1145.h"

int si1145_get_uv(void){

    int uv;
    uint16_t uv_raw;
    si1145_t sensor;

    memset(&sensor, 0, sizeof(si1145_t));

    ESP_ERROR_CHECK(si1145_init_desc(&sensor, 0, GPIO_SDA, GPIO_SCL));

    si1145_init_sensor(&sensor);

    for(int i=0; i< 10; ++i)
    {
        if (si1145_read_uv(&sensor, &uv_raw) == ESP_OK)
        {
            uv = uv_raw / 100;
        }
    }
    return uv;
}