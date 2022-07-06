idf_component_register(SRCS "main.c" "adc_readings.c" "publish.c" "storage.c" "utils.c" "wifi.c"  
    "sensors/anemometer.c" "sensors/bme680.c" "sensors/ds18b20.c" "sensors/wind_vane.c" "sensors/si1145.c"
    INCLUDE_DIRS ".")

# target_add_binary_data(${COMPONENT_TARGET} "./src/server_cert.pem" TEXT)