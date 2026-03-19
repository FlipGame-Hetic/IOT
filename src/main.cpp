#include <Arduino.h>
#include <Config.h>
#include <WiFi.h>
#include <WiFiConnection.h>
#include <MQTT.h>

WiFiConnection* wifi;
MQTT* mqtt;

void setup() {
  Serial.begin(115200);
  delay(200);
  wifi = new WiFiConnection(WIFI_SSID, WIFI_PASSWORD, WIFI_MAX_CONNECTION_RETRY);

  mqtt->begin();
}

void loop() {
  mqtt->loop();
  
}