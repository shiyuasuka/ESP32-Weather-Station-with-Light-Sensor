# ESP32-Weather-Station-with-Light-Sensor
ESP32 Weather Station with Light Sensor
Description
This project is an ESP32-based weather station with a light sensor. It displays real-time weather data along with the room's light intensity level on an ST7789 TFT LCD screen. The device connects to the OpenWeatherMap API to fetch the current weather information for Sydney. A BH1750 sensor measures the ambient light level, and a servo motor adjusts its angle based on the light level.

Features
Weather Data: Fetches current weather conditions, temperature, and humidity via OpenWeatherMap API.
Light Sensing: Uses a BH1750 sensor to measure ambient light.
Servo Control: Adjusts a servo motor based on light levels.
Display: Utilizes an ST7789 TFT LCD screen for a rich graphical interface.
Hardware Requirements
ESP32 Dev Kit
ST7789 TFT LCD Screen
BH1750 Light Sensor
Servo Motor
Software Requirements
Arduino IDE
ESP32Servo library
DFRobot_GDL library for LCD
ArduinoJson library
HTTPClient library for API calls
Setup
WiFi Configuration: Update the ssid and password in the code to connect to your WiFi network.
API Key: Replace api_key with your OpenWeatherMap API key.
Compile and Upload: Compile and upload the code to your ESP32.
Usage
Once uploaded, the device will automatically connect to the WiFi network and start fetching weather data. It will display:

Light intensity in Lux
Current weather conditions
Current temperature in Celsius
Current humidity level in percentage
The servo will adjust its angle according to the light level.
