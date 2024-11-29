#include "JsonManager.h"
#include <ArduinoJson.h>

String JsonManager::createJson(float temperature, float humidity, int light)
{
    StaticJsonDocument<200> doc;

    doc["id"] = "1140426842";

    // Propiedades
    JsonArray properties = doc.createNestedArray("properties");
    properties.add("temperatura");
    properties.add("humedad");
    properties.add("luz");

    // Valores
    JsonArray values = doc.createNestedArray("values");
    values.add(temperature);
    values.add(humidity);
    values.add(light == LOW ? 0 : 1);

    String output;
    serializeJson(doc, output);
    return output;
}
