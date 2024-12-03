#include "MQTTManager.h"
#include <PubSubClient.h>
#include <WiFi.h>

#define MQTT_BROKER "broker.mqtt.cool"
#define MQTT_PORT 1883
#define MQTT_ID "ESP32Client_publisher"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Mensaje recibido [");
    Serial.print(topic);
    Serial.print("]: ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void MQTTManager::init()
{
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
}

void MQTTManager::reconnect()
{
    unsigned long lastReconnectAttempt = 0;
    while (!mqttClient.connected())
    {
        unsigned long currentMillis = millis();
        if (currentMillis - lastReconnectAttempt >= 5000) // 5 segundos sin bloquear el flujo
        {
            lastReconnectAttempt = currentMillis;
            Serial.print("Reconectando al broker MQTT...");
            if (mqttClient.connect(MQTT_ID))
            {
                Serial.println("Conectado.");
            }
            else
            {
                Serial.print("Error, rc=");
                Serial.println(mqttClient.state());
            }
        }
        mqttClient.loop(); // Llamar a loop() para mantener la comunicación
    }
}

void MQTTManager::publish(String topic, String payload)
{
    Serial.println("------Enviando datos a MQTT--------:");
    Serial.println(payload);
    if (mqttClient.publish(topic.c_str(), payload.c_str()))
    {
        Serial.println("Datos enviados a MQTT.");
    }
    else
    {
        Serial.println("Error al enviar datos a MQTT.");
    }
}

bool MQTTManager::isConnected()
{
    return mqttClient.connected();
}

void MQTTManager::loop()
{
    mqttClient.loop();
}