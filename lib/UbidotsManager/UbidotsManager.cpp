#include "UbidotsManager.h"

UbidotsManager::UbidotsManager(const char *wifiSSID, const char *wifiPassword, const char *ubidotsToken,
                               const char *device, const char *varTemp, const char *varHum)
    : ssid(wifiSSID), password(wifiPassword), token(ubidotsToken),
      deviceLabel(device), variableLabelTemp(varTemp), variableLabelHum(varHum) {}

void UbidotsManager::connectWiFi()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectado a WiFi");
}

void UbidotsManager::sendToUbidots(float temperature, float humidity)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String url = String("http://industrial.api.ubidots.com/api/v1.6/devices/") + deviceLabel;
        http.begin(url.c_str());
        http.addHeader("Content-Type", "application/json");
        http.addHeader("X-Auth-Token", token);

        String payload = "{\"temperatura\":" + String(temperature) + ",\"humedad\":" + String(humidity) + "}";
        int httpResponseCode = http.POST(payload);
        http.end();

        if (httpResponseCode > 0)
        {
            Serial.println("Datos enviados a Ubidots");
        }
        else
        {
            Serial.println("Error al enviar datos");
        }
    }
}
