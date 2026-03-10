#include <WiFiConnection.h>
#include <Log.h>

WiFiConnection::WiFiConnection(const char* SSID, const char* Password, uint16_t WifiRetries) :
    _ssid(SSID),
    _password(Password),
    _maxRetries(WifiRetries)
{
    LOG_WIFI("Starting Wifi...");
    LOG_WIFIF("Connecting to %s", _ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    uint8_t retries = 0;

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        retries++;
        if (retries >= _maxRetries) {    
            LOG_WIFI("Wifi Connection failed, please verify the SSID or pwd");
            while (1)
            {
                Serial.print(".");
                delay(1000);
            };
        };
        Serial.println("");
    };
    
    LOG_WIFI("Connection successfull !");
}