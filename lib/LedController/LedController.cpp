#include "LedController.h"
#include <Arduino.h>

LedController::LedController(int pin) : ledPin(pin) {}

void LedController::begin()
{
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW); // Apaga el LED al iniciar
}

void LedController::turnOn()
{
    digitalWrite(ledPin, HIGH);
}

void LedController::turnOff()
{
    digitalWrite(ledPin, LOW);
}
