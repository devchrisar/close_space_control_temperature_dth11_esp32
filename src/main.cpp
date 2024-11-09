#include <Arduino.h>
#include "SensorManager.h"
#include "LedController.h"

// Configuración del sensor y LEDs
#define DHT_PIN 15
#define DHT_TYPE DHT11
#define LED_ROJO_PIN 5
#define LED_AZUL_PIN 4

// Umbrales de temperatura y humedad
const int TEMPERATURA_UMBRAL = 30;
const int HUMEDAD_MINIMA = 40;
const int HUMEDAD_MAXIMA = 60;

// Instancias de las clases
SensorManager sensor(DHT_PIN, DHT_TYPE);
LedController ledRojo(LED_ROJO_PIN);
LedController ledAzul(LED_AZUL_PIN);

void setup()
{
  Serial.begin(115200);
  sensor.begin();
  ledRojo.begin();
  ledAzul.begin();
}

void loop()
{
  // Obtener lecturas del sensor
  float temperatura = sensor.getTemperature();
  float humedad = sensor.getHumidity();

  // Validar lectura de temperatura
  if (temperatura == -1.0)
  {
    Serial.println("Error al leer la temperatura.");
  }
  else
  {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println("°C");
  }

  // Validar lectura de humedad
  if (humedad == -1.0)
  {
    Serial.println("Error al leer la humedad.");
  }
  else
  {
    Serial.print("Humedad: ");
    Serial.print(humedad);
    Serial.println("%");
  }

  // Control del LED rojo (temperatura)
  if (temperatura > TEMPERATURA_UMBRAL)
  {
    ledRojo.turnOn();
  }
  else
  {
    ledRojo.turnOff();
  }

  // Control del LED azul (humedad)
  if (humedad >= HUMEDAD_MINIMA && humedad <= HUMEDAD_MAXIMA)
  {
    ledAzul.turnOn();
  }
  else
  {
    ledAzul.turnOff();
  }

  delay(3000); // Espera de 3 segundos antes de la próxima lectura
}
