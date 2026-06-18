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
  
  Button secondaryButtonRight(SECONDARY_BUTTON_RIGHT_PIN);
  Button secondaryButtonLeft(SECONDARY_BUTTON_LEFT_PIN);

  Button buttonTop(FRONT_PANEL_BUTTON_TOP_PIN);
  Button buttonMiddle(FRONT_PANEL_BUTTON_MIDDLE_PIN);
  Button buttonBottom(FRONT_PANEL_BUTTON_BOTTOM_PIN);

  Button underPlunger(FRONT_PANEL_BUTTON_UNDERPLUNGER_PIN);
  Button plunger(PLUNGER_PIN);

  // ===== Events =====

  void publishEvent(const char *id, JsonDocument doc, const char *topic) {
    LOG_MAINF("Pressed %s", id);
    mqtt.publishJson(topic, doc);
  }
  
  void publishButtonEvent(const char *id, uint8_t state) {
    JsonDocument doc;
    doc["id"] = id;
    doc["state"] = state;
    doc["ts"] = millis();

    publishEvent(id, doc, "pinball/esp32-test/input/button");
  }

  void publishPlungerEvent() {
    LOG_MAIN("Plunger Pressed");

    JsonDocument doc;
    doc["state"] = 1; // Hardcoded because the plunger is broken irl
    doc["ts"] = millis();
    mqtt.publishJson("pinball/esp32-test/input/plunger", doc);
  }

  // ===== Helpers =====

  void bindButtonPress(Button& btn, const char *id) {
    btn.onPress([id]() { publishButtonEvent(id, 1); });
    btn.onPress([id]() { publishButtonEvent(id, 0); });
  }

  // ===== Callback Func =====

  void onPlungerClick() {
    publishPlungerEvent();
  }
  
  // ===== Setup =====
  
  void setup() {
    Serial.begin(115200);
    delay(200);

    // Startup all the requiered elements
    wifi.begin();

    Serial.println(WiFi.localIP());

    mqtt.begin();

    primaryButtonLeft.begin();
    primaryButtonRight.begin();

    secondaryButtonLeft.begin();
    secondaryButtonRight.begin();
    
    buttonTop.begin();
    buttonMiddle.begin();
    buttonBottom.begin();

    underPlunger.begin();
    plunger.begin();


    
    // Bind the callbacks
    bindButtonPress(primaryButtonLeft, "L1");
    bindButtonPress(primaryButtonRight, "R1");

    bindButtonPress(secondaryButtonLeft, "L2");
    bindButtonPress(secondaryButtonRight, "R2");

    bindButtonPress(buttonTop, "top");
    bindButtonPress(buttonMiddle, "middle");
    bindButtonPress(buttonBottom, "bottom");

    bindButtonPress(underPlunger, "under_plunger");

    plunger.onClick(onPlungerClick);
  }


  // ===== Loop =====

  void loop() {
    mqtt.loop();
    primaryButtonLeft.update();
    primaryButtonRight.update();

    secondaryButtonLeft.update();
    secondaryButtonRight.update();

    buttonTop.update();
    buttonMiddle.update();
    buttonBottom.update();

    underPlunger.update();
    plunger.update();
  }