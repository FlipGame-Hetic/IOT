#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WiFiConnection {
    public :
        WiFiConnection(const char* SSID, const char* Password, uint16_t WifiRetries = 0);   

        void begin();
    private :
        const char* _ssid;
        const char* _password;
        uint8_t _maxRetries;
};