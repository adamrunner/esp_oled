## ESP8266 (ESP-12/NodeMCU) temperature server w/OLED display

A small temperature sensor that will display the temperature of the room, in addition to the IP Address of the device.

Also runs a web server that will respond to two routes
- `http://esp_ip_address/` - Displays a text only web page with the current temperature in a human readable format, and degrees in fahrenheit.
- `htttp://esp_id_address/temp` - Displays _just_ the number of the current degrees fahrenheit, I use this URL to scrape data from the sensor and log it elsewhere.

Requires several libraries for the Arduino IDE, these are all available in the Arduino IDE. 
- ESP8266WiFi
- Wire
- Adafruit_GFX
- Adafruit_SSD1306
- ESP8266WebServer
- FreeSans24pt7b
- OneWire
- DallasTemperature
