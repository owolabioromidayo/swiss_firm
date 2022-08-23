#ifndef PROJ_GUVA_H_
#define PROJ_GUVA_H_

#include "driver/adc.h"
#include "guva_s12sd.h"
#include <esp_err.h>
#include <stdio.h>

#include "../config.h"

int get_uv_values(void);

#endif // PROJ_GUVA_H_