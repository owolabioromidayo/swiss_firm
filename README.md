# swiss_firm
ESP32 Firmware for SWISS (Solar Weather Information Station) Project 

Project Website: [Link](https://sites.google.com/view/swiss-uidesign/home)


<img src="assets/top.jpg?raw=true" />
<br />
<img src="assets/back.jpg?raw=true" />
<br />

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

# Schematics

<img src="assets/schematics.png?raw=true" />
<img src="assets/pinout.jpg?raw=true" />

# Setup 

[Guide](https://sites.google.com/view/swiss-uidesign/guide/firmware-setup)
- Clone this repo
- Move main/config_example.h to main/config.h.
- If your hardware connections to the ESP32 are the same as the schematic in the previous section,then you have nothing to edit. If not, edit the pin configurations.
- Install the ESP-IDF Framework: [Link](https://dl.espressif.com/dl/esp-idf/) 
- Install the CP210x USB-UART Driver: [Link](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)  
 - Connect your ESP32 to your PC via USB. Open Device Manager and confirm that the driver works.
 - Open the ESP_IDF CMD tool.
 - Navigate to the firmware folder.
 - Run idf.py flash build monitor. If you keep getting flash errors, try holding down the buttons on your ESP32. The left button is for restarting and the right button     is for flashing.
 - The code should be configured in test mode so you should see the ESP32 attempt reads from the sensors. If this is successful, then you are more than halfway done.       If not, your hardware connections might contain some mistakes.
 - If you are experiencing brownout errors, your hardware connections are at fault.
 - Now set TEST_MODE in your config.h file to 0 and HOSTNAME to the endpoint of your main service with the publish password i.e. herokusite.com/publish?password=PUBLISH_PASSWORD. 
 - Navigate to src/main.c and change SLEEP_TIME_MIN to 1 so we can test the whole thing.
 - Recompile and run your code with idf.py flash build monitor.  
 - Now shake your rain gauge to trigger an interrupt. You should see the system boot back up and record this. 
 - Wait for 1 minute and the system should create a hotspot.
 - Connect to this hotspot with the default password in lib/esp32-wifi-manager/Kconfig which is esp32pwd.  You can change this value along with the hotspot name from here if you wish.
 - Go to the website 10.10.0.1 while connected to the hotspot and fill in your Wifi Details.
 - The system should now attempt to collect all sensor values and post them to the main service. 
 - At this point. the only other thing that is untested is the wind vane so you should see logs of the wind direction in the terminal before the system posts data.
If everything goes well, the data should be posted to the main service. You can confirm that the whole system works by checking your database information
Now that everything is working properly, set SLEEP_TIME_MIN to 30.  




