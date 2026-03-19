#pragma once

#include <Arduino.h>
#include <Log.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <map>

using RawCallback = std::function<void(const String&)>;
using JsonCallback = std::function<void(JsonDocument&)>;


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
        
        void publish(const char* topic, const char* payload, bool retained = false);
        void publishJson(const char* topic, const JsonDocument& doc, bool retained = false);

        void subscribeRaw(const char* topic, RawCallback cb, uint8_t qos = 0);
        void subscribeJson(const char* topic, JsonCallback cb, uint8_t qos = 0);

    private:
        std::map<String, RawCallback> _rawHandlers;
        std::map<String, JsonCallback> _jsonHandlers;

        WiFiClient _espClient;
        PubSubClient _mqtt;

        String _id;
        String _broker;
        String _statusTopic;
        String _user;
        String _pass;

        uint16_t _port;

        void _dispatch(char* topic, byte* payload, unsigned int length);
};