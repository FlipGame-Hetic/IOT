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

void MQTT::subscribe(const char* topic, uint8_t qos) {
    _mqtt.subscribe(topic, qos);
}
