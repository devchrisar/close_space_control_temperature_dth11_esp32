#ifndef PWM_MANAGER_H
#define PWM_MANAGER_H

class PWMManager
{
public:
    static int calculatePWM(float temperature, float humidity, int light);
};

#endif