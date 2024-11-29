#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <Arduino.h>

class JsonManager
{
public:
    String createJson(float temperature, float humidity, int light);
};

#endif
