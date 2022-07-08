#include "sensors/ds18b20.h"
#include "sensors/bme680.h"
#include "sensors/si1145.h"
#include "sensors/wind_vane.h"
#include "adc_readings.h"

#include "types.h"

void test_bme680(void);
void test_ds18b20(void);
void test_si1145(void);
void test_wind_vane(void);
void test_battery_adc(void);

void test_all();
