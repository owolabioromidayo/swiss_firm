#include "./bme680.h"
#include <i2cdev.h>

bme680_values_float_t bme680_get_values(void){

    bme680_values_float_t values;
    bme680_t sensor;

    i2cdev_init();

    memset(&sensor, 0, sizeof(bme680_t));

    ESP_ERROR_CHECK(bme680_init_desc(&sensor, BME680_I2C_ADDR_0, 0, GPIO_SDA, GPIO_SCL));

    bme680_init_sensor(&sensor);

    // Changes the oversampling rates to 4x oversampling for temperature
    // and 2x oversampling for humidity and pressure.
    bme680_set_oversampling_rates(&sensor, BME680_OSR_4X, BME680_OSR_2X, BME680_OSR_2X);

    // Change the IIR filter size for temperature and pressure to 7.
    bme680_set_filter_size(&sensor, BME680_IIR_SIZE_7);

    // Change the heater profile 0 to 200 degree Celsius for 100 ms.
    bme680_set_heater_profile(&sensor, 0, 200, 100);
    bme680_use_heater_profile(&sensor, 0);

    // Set ambient temperature to 10 degree Celsius
    bme680_set_ambient_temperature(&sensor, 10);

    // as long as sensor configuration isn't changed, duration is constant
    uint32_t duration;
    bme680_get_measurement_duration(&sensor, &duration);

    // TickType_t last_wakeup = xTaskGetTickCount();

    for(int i=0; i< 10; ++i)
    {
        // trigger the sensor to start one TPHG measurement cycle
        if (bme680_force_measurement(&sensor) == ESP_OK)
        {
            // passive waiting until measurement results are available
            vTaskDelay(duration);

            // get the results and do something with them
            if (bme680_get_results_float(&sensor, &values) == ESP_OK)
                // printf("BME680 Sensor: %.2f °C, %.2f %%, %.2f hPa, %.2f Ohm\n",
                //         values.temperature, values.humidity, values.pressure, values.gas_resistance);
                continue; // no more printing values
        }
    }
    return values;
}