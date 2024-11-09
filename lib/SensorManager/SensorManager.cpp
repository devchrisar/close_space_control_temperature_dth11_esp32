#include "SensorManager.h"

SensorManager::SensorManager(int sensorPin, int sensorType) : dht(sensorPin, sensorType)
{
    pin = sensorPin;
}

void SensorManager::begin()
{
    dht.begin();
}

float SensorManager::getTemperature()
{
    float temp = dht.readTemperature();
    return isnan(temp) ? -1.0 : temp;
}

float SensorManager::getHumidity()
{
    float hum = dht.readHumidity();
    return isnan(hum) ? -1.0 : hum;
}
