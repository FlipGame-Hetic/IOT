#include <MQTT.h>

MQTT::MQTT(
    const char* id, 
    const char* broker, 
    const char* user,
    const char* pass,
    const char* statusTopic,
    uint16_t port
) : 
    _mqtt(_espClient),
    _id(id),
    _broker(broker),
    _user(user),
    _pass(pass),
    _statusTopic(statusTopic),
    _port(port)
{}


void MQTT::begin() {
    _mqtt.setServer(_broker.c_str(), _port);
    _mqtt.setCallback([this](char* topic, byte* payload, unsigned int length) {
        this->_dispatch(topic, payload, length);
    }); 
    reconnect();
}

void MQTT::loop() {
    if (!_mqtt.connected()) {
        reconnect();
    }

    _mqtt.loop();
}

void MQTT::reconnect() {
    while(!_mqtt.connected()) {
        LOG_MQTT("Connecting to MQTT...");

        // Last Will and Testament
        bool connected = _mqtt.connect(
            _id.c_str(),
            _user.c_str(),
            _pass.c_str(),
            _statusTopic.c_str(),
            1,
            true,
            "offline"
        );

        if (connected) {
            LOG_MQTT("Connection successfull !");
            _mqtt.publish(_statusTopic.c_str(), "online", true);

            // Resubscribre to all topics
            for (auto& pair : _jsonHandlers) {
                _mqtt.subscribe(pair.first.c_str());    
            }
            for (auto& pair : _rawHandlers) {
                _mqtt.subscribe(pair.first.c_str());            
            }
        } else {
            LOG_MQTT_ERR("Failed :");
            LOG_MQTT_ERR(_mqtt.state());
            LOG_MQTT_ERR("Retrying in 5s");
            delay(5000);
        }
    }
}


void MQTT::publish(const char* topic, const char* payload, bool retained) {
    bool status = _mqtt.publish(topic, payload, retained);

    if (status) {
        LOG_MQTTF("Data sent on : %s", topic);
    } else {
        LOG_MQTT_ERRF("Error sending data on : %s", topic);
    }
}

void MQTT::publishJson(const char* topic, const JsonDocument& doc, bool retained) {
    char buffer[512];
    size_t n = serializeJson(doc, buffer, sizeof(buffer));

    if (n == 0) {
        LOG_MQTT_ERR("JSON serialization failed");
        return;
    }

    publish(topic, buffer, retained);
}

void MQTT::subscribeJson(const char* topic, JsonCallback cb, uint8_t qos) {
    _jsonHandlers[String(topic)] = cb;
    _mqtt.subscribe(topic, qos);
}

void MQTT::subscribeRaw(const char* topic, RawCallback cb, uint8_t qos) {
    _rawHandlers[String(topic)] = cb;
    _mqtt.subscribe(topic, qos);
}

void MQTT::_dispatch(char* topic, byte* payload, unsigned int length) {
    // Convert the payload in a String
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    String topicStr(topic);

    // Check if the topic require a jsonHandler
    auto jsonIt = _jsonHandlers.find(topicStr);
    if (jsonIt != _jsonHandlers.end()){
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, message);
        if (error) {
            LOG_MQTT_ERRF("JSON parse failed on %s: %s", topic, error.c_str());
            return;
        }
        jsonIt->second(doc);
        return;
    }

    // Else it will be checked in the raw handler
    auto rawIt = _rawHandlers.find(topicStr);
    if (rawIt != _rawHandlers.end()) {
        rawIt->second(String(message));
        return;
    }

    // If the topic isnt present in any of the previous handler return an error
    LOG_ERRORF("No handler found for : %s", topic);
}