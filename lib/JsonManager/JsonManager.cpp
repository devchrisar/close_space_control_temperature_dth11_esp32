#include "JsonManager.h"
#include <ArduinoJson.h>

String JsonManager::createJson(float temperature, float humidity, int light)
{
    DynamicJsonDocument doc(200);

    doc["id"] = "1140426842";

    // Propiedades
    JsonArray properties = doc["properties"].to<JsonArray>();
    properties.add("temperatura");
    properties.add("humedad");
    properties.add("luz");

    // Valores
    JsonArray values = doc["values"].to<JsonArray>();
    values.add(temperature);
    values.add(humidity);
    values.add(light);

    String output;
    serializeJson(doc, output);
    return output;
}

String JsonManager::createPWMJson(int pwm)
{
    DynamicJsonDocument doc(100);

    doc["id"] = "1140426842";
    doc["pwm"] = pwm;

    String output;
    serializeJson(doc, output);
    return output;
}