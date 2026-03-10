#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WiFiConnection {
    public :
        WiFiConnection(const char* SSID, const char* Password, uint16_t WifiRetries = 5);   

    private : 
        String _ssid;
        String _password;
        uint8_t _maxRetries;
};