# swiss_firm
ESP32 Firmware for SWISS (Solar Weather Information Station) Project 

Project Website: [Link](https://sites.google.com/view/swiss-uidesign/home)

# Features
- Power Conservation with Deep Sleep Mode Switching
- Dynamic WiFi Configuration and Connection, Website, with [esp32-wifi-manager](https://github.com/owolabioromidayo/esp32-wifi-manager)
- HTTPS Sensor Data Publishing 
- Sensors
  - BME680 : Humidity, Pressure, Temperature, Air Quality Sensor
  - Battery Percentage Measurements using Voltage Dividers and ADC
  - Anemometer: Wind Speeed using GPIO tick counting routine and conversion.
  - Wind Vane : Wind Direction using resistor network and ADC Voltage Level Conversion.
  - DS18B20 : Ext Temp Sensor using OnewWire configuration.
  - GUVA-S12SD - UV Index Sensor using ADC. Wrote Custom Driver : [Link](https://github.com/owolabioromidayo/GUVA_S12SD)
  - SI1145 - UV Sensor using I2C. Wrote Custom Driver: [Link](https://github.com/owolabioromidayo/Si1145)


# Stack
- ESP32
- C
- FreeRTOS : [Link](https://freertos.org/)


# Setup 

[Guide](https://sites.google.com/view/swiss-uidesign/guide/firmware-setup)

<img src="assets/top.jpg?raw=true" />
<br />
<img src="assets/back.jpg?raw=true" />
<br />


