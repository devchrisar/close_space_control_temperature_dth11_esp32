#include "UbidotsManager.h"

UbidotsManager::UbidotsManager(const char *wifiSSID, const char *wifiPassword, const char *ubidotsToken,
                               const char *device, const char *varTemp, const char *varHum, const char *varLight)
    : ssid(wifiSSID), password(wifiPassword), token(ubidotsToken),
      deviceLabel(device), variableLabelTemp(varTemp), variableLabelHum(varHum), variableLabelLight(varLight) {}

void UbidotsManager::connectWiFi()
{
    WiFi.begin(ssid, password);
    unsigned long startMillis = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startMillis < 10000) // 10 segundos de espera
    {
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nConectado a WiFi");
    }
    else
    {
        Serial.println("\nFallo la conexión a WiFi");
    }
}

void UbidotsManager::sendToUbidots(float temperature, float humidity, int lightState)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String url = String("http://industrial.api.ubidots.com/api/v1.6/devices/") + deviceLabel;
        http.begin(url.c_str());
        http.addHeader("Content-Type", "application/json");
        http.addHeader("X-Auth-Token", token);

        String payload = "{\"" + String(variableLabelTemp) + "\":" + String(temperature) + "," +
                         "\"" + String(variableLabelHum) + "\":" + String(humidity) + "," +
                         "\"" + String(variableLabelLight) + "\":" + String(lightState) + "}";
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
