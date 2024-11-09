#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <DHT.h>

class SensorManager
{
private:
    DHT dht;
    int pin;

public:
    SensorManager(int sensorPin, int sensorType);
    void begin();
    float getTemperature();
    float getHumidity();
};

#endif // SENSOR_MANAGER_H
