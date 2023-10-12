#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <BH1750.h>
#include <ESP32Servo.h>
#include "DFRobot_GDL.h"
#include <MQTT.h>

#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3

const char* ssid = "iPhone";
const char* password = "12345678";
const char* mqtt_server = "mqtt.eclipseprojects.io";
const char* mqtt_topic = "web_mqtt";
const char* api_key = "ed94c71e2aa77121a9923c531bc3d7eb";
String api_url = "http://api.openweathermap.org/data/2.5/weather?q=Sydney&appid=" + String(api_key);

BH1750 lightMeter;
Servo myservo;
DFRobot_ST7789_240x320_HW_SPI screen(TFT_DC, TFT_CS, TFT_RST);
WiFiClient espClient;
MQTTClient mqttClient(256);

unsigned long lastApiCallTime = 0;
unsigned long lastLuxUpdateTime = 0;  // 用于lux更新的时间变量

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  mqttClient.begin(mqtt_server, 1883, espClient);
  if (mqttClient.connect("ESP32Client")) {
    Serial.println("Connected to MQTT broker!");
  } else {
    Serial.println("Failed to connect to MQTT broker.");
  }

  Wire.begin();
  lightMeter.begin();
  delay(100);
  myservo.attach(13);

  screen.begin();
  screen.fillScreen(COLOR_RGB565_WHITE);
  screen.setTextWrap(false);
  screen.setTextColor(COLOR_RGB565_GREEN);
  screen.setTextSize(2);

  screen.setCursor(16, 16);
  screen.print("Light Intensity:");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastLuxUpdateTime > 1000) {  // 1000 milliseconds
    lastLuxUpdateTime = currentMillis;

    uint16_t lux = lightMeter.readLightLevel();
    // 更新光强值
    screen.fillRect(16, 51, screen.width() - 32, 35, COLOR_RGB565_WHITE);
    screen.setCursor(16, 51);
    screen.print(lux);
  }

  if (currentMillis - lastApiCallTime > 60000) {  // One minute
    lastApiCallTime = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(api_url);
      int httpCode = http.GET();

      if (httpCode > 0) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        const char* weather = doc["weather"][0]["main"];
        float temp = doc["main"]["temp"].as<float>() - 273.15;
        int humidity = doc["main"]["humidity"];
        uint16_t lux = lightMeter.readLightLevel();

        String mqttMessage = "Weather: " + String(weather) + ", Temp: " + String(temp) + " C, Humidity: " + String(humidity) + "%, Lux: " + String(lux);
        mqttClient.publish(mqtt_topic, mqttMessage);

        screen.fillRect(16, 86, screen.width() - 32, 35 * 3, COLOR_RGB565_WHITE);

        screen.setCursor(16, 86);
        screen.print("Weather: ");
        screen.print(weather);

        screen.setCursor(16, 121);
        screen.print("Temp: ");
        screen.print(temp);
        screen.print(" C");

        screen.setCursor(16, 156);
        screen.print("Humidity: ");
        screen.print(humidity);
        screen.print("%");

        screen.setCursor(16, 191);
        screen.print("City: Sydney");
      }

      http.end();
    }
  }
  mqttClient.loop();
}
