#ifndef PROJ_STORAGE_H_
#define PROJ_STORAGE_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_OK 0
#define STORAGE_ERR 1


void init_storage(void);
int get_string_value_nvs(const char* key, char* out_value, size_t length);
int set_string_value_nvs(const char* key, char* value);


#endif // PROJ_STORAGE_H_