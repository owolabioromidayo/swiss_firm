#include "guvas12sd.h"

int get_uv_values(void)
{
  guva_s12sd_t sensor = guva_s12sd_init_desc(ADC_UV_PIN, 3.3, 50);

  int uv;
  if (guva_s12sd_read_uv_index(&sensor, &uv))
  {
    printf("UV Index: %d \n", uv);
    return uv;
  }
  printf("Failed to read value\n");
  return -1;

}

