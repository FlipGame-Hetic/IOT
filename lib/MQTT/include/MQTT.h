#pragma once

#include <Arduino.h>
#include <Log.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

class MQTT {
    public:
        MQTT(
            const char* id, 
            const char* broker, 
            const char* statusTopic,
            const char* user,
            const char* pass,
            uint16_t port
        );
        
        void begin();
        void loop();
        void reconnect();
        
        void publish(const char* topic, const char* payload, bool retained);
        void publishJson(const char* topic, const JsonDocument& doc, bool retained);

        void subscribe(const char* topic, uint8_t qos);

    private:
        WiFiClient _espClient;
        PubSubClient _mqtt;

        String _id;
        String _broker;
        String _statusTopic;
        String _user;
        String _pass;

        uint16_t _port;
};