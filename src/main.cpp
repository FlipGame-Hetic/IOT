  #include <Arduino.h>
  #include <Config.h>
  #include <WiFi.h>
  #include <WiFiConnection.h>
  #include <MQTT.h>
  #include <Button.h>

  // WiFiConnection wifi(WIFI_SSID, WIFI_PASSWORD, WIFI_MAX_CONNECTION_RETRY);
  // MQTT mqtt(
  //     "esp32-test",
  //     "192.168.1.xx",
  //     "",
  //     "",
  //     "home/esp32/status",
  //     1883
  // );

  Button buttonTest(BUTTON_PIN);
  
  void onButtonClick() {
    Serial.printf("Click");
  }

  void onButtonDoubleClick() {
    Serial.printf("DoubleClick");
  }

  void onButtonPress() {
    Serial.printf("Press");
  }

  void onLongPress() {
    Serial.printf("Long Press");
  }

  void onRelease() {
    Serial.printf("Release");
  }
  
  void setup() {
    Serial.begin(115200);
    delay(200);

    // wifi.begin();

    // Serial.println(WiFi.localIP());

    // mqtt.begin();

    buttonTest.begin();
    buttonTest.onClick(onButtonClick);
    buttonTest.onDoubleClick(onButtonDoubleClick);
    buttonTest.onLongPress(onLongPress);
    buttonTest.onRelease(onRelease);
  }

  void loop() {
    // mqtt.loop();
    buttonTest.update();
  }