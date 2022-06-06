idf_component_register(SRCS "main.c" "adc_readings.c" "publish.c" "storage.c" "utils.c" "wifi.c" 
    "sensors/anemometer.c" "sensors/bme680.c" "sensors/ds18b20.c" "sensors/raingauge.c" "sensors/wind_vane.c"
    INCLUDE_DIRS ".")

# target_add_binary_data(${COMPONENT_TARGET} "./src/server_cert.pem" TEXT)