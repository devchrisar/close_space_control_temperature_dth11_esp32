#ifndef UBIDOTS_MANAGER_H
#define UBIDOTS_MANAGER_H

#include <WiFi.h>
#include <HTTPClient.h>

class UbidotsManager
{
private:
    const char *ssid;
    const char *password;
    const char *token;
    const char *deviceLabel;
    const char *variableLabelTemp;
    const char *variableLabelHum;
    const char *variableLabelLight;

public:
    UbidotsManager(const char *ssid, const char *password, const char *token,
                   const char *deviceLabel, const char *variableLabelTemp, const char *variableLabelHum, const char *variableLabelLight);
    void connectWiFi();
    void sendToUbidots(float temperature, float humidity, int lightState);
};

#endif // UBIDOTS_MANAGER_H
