#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>

class MQTTManager
{
public:
    void init();
    void reconnect();
    void publish(String topic, String payload);
    bool isConnected();
    void loop();
};

#endif
