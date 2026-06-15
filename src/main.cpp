  #include <Arduino.h>
  #include <Config.h>
  #include <WiFi.h>
  #include <WiFiConnection.h>
  #include <MQTT.h>
  #include <Button.h>

  WiFiConnection wifi(WIFI_SSID, WIFI_PASSWORD, WIFI_MAX_CONNECTION_RETRY);
  MQTT mqtt(
      "esp32-test",
      "100.125.185.88",
      "pinball/esp32-test/esp32/status",
      "",
      "",
      1883
  );

  Button primaryButtonLeft(PRIMARY_BUTTON_LEFT_PIN);
  Button primaryButtonRight(PRIMARY_BUTTON_RIGHT_PIN);
  Button plunger(PLUNGER_PIN);

  uint32_t plungerStart = 0;
  const uint32_t plungerMax = 2000;

  void onButtonDoubleClick() {
    Serial.printf("DoubleClick");
  }

  void onPrimaryButtonLeftPress() {
    Serial.printf("Press Left");
    JsonDocument doc;
    doc["id"] = "flipper_left";
    doc["state"] = 1;
    doc["ts"] = millis();
    mqtt.publishJson("pinball/esp32-test/input/button", doc);
  }

  void onPrimaryButtonRightPress() {
    Serial.printf("Press Right");
    JsonDocument doc;
    doc["id"] = "flipper_right";
    doc["state"] = 1;
    doc["ts"] = millis();
    mqtt.publishJson("pinball/esp32-test/input/button", doc);
  }

  void onPlungerPress() {
    Serial.println("Plunger Pressed");
    plungerStart = millis();
  }

  void onLongPress() {
    Serial.printf("Long Press");
  }

  void onPrimaryButtonLeftRelease() {
    Serial.printf("Release Left");
    JsonDocument doc;
    doc["id"] = "flipper_left";
    doc["state"] = 0;
    doc["ts"] = millis();
    mqtt.publishJson("pinball/esp32-test/input/button", doc);
  }

  void onPrimaryButtonRightRelease() {
    Serial.printf("Release Right");
    JsonDocument doc;
    doc["id"] = "flipper_right";
    doc["state"] = 0;
    doc["ts"] = millis();
    mqtt.publishJson("pinball/esp32-test/input/button", doc);
  }

  void onPlungerRelease() {
    Serial.printf("Release Plunger");
    uint32_t held = millis() - plungerStart;
    float position = (float)std::min(held, plungerMax) / (float)plungerMax;

    JsonDocument doc;
    doc["position"] = position;
    doc["released"] = true;
    doc["ts"] = millis();
    mqtt.publishJson("pinball/esp32-test/input/plunger", doc);
  }
  
  void setup() {
    Serial.begin(115200);
    delay(200);

    wifi.begin();

    Serial.println(WiFi.localIP());

    mqtt.begin();

    primaryButtonLeft.begin();
    primaryButtonRight.begin();
    plunger.begin();

    primaryButtonLeft.onPress(onPrimaryButtonLeftPress);
    primaryButtonRight.onPress(onPrimaryButtonRightPress);

    primaryButtonLeft.onRelease(onPrimaryButtonLeftRelease);
    primaryButtonRight.onRelease(onPrimaryButtonRightRelease);

    plunger.onPress(onPlungerPress);
    plunger.onRelease(onPlungerRelease);
  }

  void loop() {
    mqtt.loop();
    primaryButtonLeft.update();
    primaryButtonRight.update();
    plunger.update();
  }