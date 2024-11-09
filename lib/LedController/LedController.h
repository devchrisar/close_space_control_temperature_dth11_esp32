#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

class LedController
{
private:
    int ledPin;

public:
    LedController(int pin);
    void begin();
    void turnOn();
    void turnOff();
};

#endif // LED_CONTROLLER_H
