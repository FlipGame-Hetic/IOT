#include <WiFiConnection.h>
#include <Log.h>

WiFiConnection::WiFiConnection(
    const char* SSID, 
    const char* Password, 
    uint16_t WifiRetries
) :
    _ssid(SSID),
    _password(Password),
    _maxRetries(WifiRetries)
{}

void WiFiConnection::begin() {
    LOG_WIFI("Starting Wifi...");
    LOG_WIFIF("Connecting to %s", _ssid);

    // Set the params for the wifi and try to connect
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    uint8_t retries = 0;
    uint8_t countRetries = 0;

    // Retry the wifi connection until the max repetition is reached (_maxRetries = 0 -> run without stopping)
    while (WiFi.status() != WL_CONNECTED) {
        
        // Log infos about its current state
        if (countRetries >= 10) {
            LOG_WIFI("Trying to connect...");
            Serial.println("");
        };

        delay(400);

        LOG_WIFI(".");
        retries++;
        
        if (_maxRetries != 0 and retries >= _maxRetries ) {    
            LOG_WIFI("Wifi Connection failed, please verify the SSID or pwd");
            while (1)
            {
                LOG_WIFI_ERR("Failed connection... waiting for restart");
                delay(10000);
            };
        };
    };
    
    LOG_WIFI("Connection successfull !");
};