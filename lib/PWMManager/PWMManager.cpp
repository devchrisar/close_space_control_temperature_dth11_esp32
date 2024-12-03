#include "PWMManager.h"
#include <Arduino.h>

int PWMManager::calculatePWM(float temperature, float humidity, int light)
{
    // Normalizar los valores
    float tempNormalized = constrain((temperature - 20) / 10.0, 0, 1);  // 20°C a 30°C → 0 a 1
    float humidityNormalized = constrain((humidity - 40) / 20.0, 0, 1); // 40% a 60% → 0 a 1
    float lightNormalized = constrain((4095 - light) / 4095.0, 0, 1);   // Invertir escala: más luz → mayor valor

    // Peso para cada factor
    float weightTemp = 0.5;     // 50% de peso
    float weightHumidity = 0.2; // 20% de peso
    float weightLight = 0.3;    // 30% de peso

    // Calcular PWM como combinación ponderada
    float pwmValue = (weightTemp * tempNormalized + weightHumidity * humidityNormalized + weightLight * lightNormalized) * 100;
    return (int)constrain(pwmValue, 0, 100); // Escalar a rango 0-100
}

// Temperatura	Humedad	    Luz	        PWM	            Descripción
// Alta (1)	    Alta (1)	Alta (1)	100	    Todas las variables al máximo.
// Baja (0)	    Baja (0)	Baja (0)	0	    Todas las variables al mínimo.
// Alta (1)	    Baja (0)	Alta (1)	80	    Dominan temperatura y luz.
// Baja (0)	    Alta (1)	Alta (1)	50	    Luz domina, humedad alta contribuye algo.
// Alta (1)	    Alta (1)	Baja (0)	70	    Dominan temperatura y humedad.
// Media (0.5)	Media (0.5)	Media (0.5)	50	    Todos los factores contribuyen de manera equilibrada.
// Alta (1)	    Baja (0)	Baja (0)	50	    Solo temperatura contribuye significativamente.