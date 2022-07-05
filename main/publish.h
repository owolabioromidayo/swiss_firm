#ifndef PROJ_PUBLISH_H_
#define PROJ_PUBLISH_H_

#include "sensors/ds18b20.h"
#include "sensors/bme680.h"
#include "sensors/anemometer.h"
#include "sensors/si1145.h"
#include "sensors/wind_vane.h"
#include "adc_readings.h"

#include "types.h"

sensor_values_t gather_data();

#endif //PROJ_PUBLISH_H_