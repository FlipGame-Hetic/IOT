#include <Arduino.h>
#include <Config.h>
#include <WiFi.h>
#include <WiFiConnection.h>
#include <MQTT.h>

WiFiConnection wifi(WIFI_SSID, WIFI_PASSWORD, WIFI_MAX_CONNECTION_RETRY);
MQTT mqtt(
    "esp32-test",
    "192.168.1.xx",
    "",
    "",
    "home/esp32/status",
    1883
);

void setup() {
  Serial.begin(115200);
  delay(200);

  wifi.begin();

  Serial.println(WiFi.localIP());

  mqtt.begin();
}

void loop() {
  mqtt.loop();
}