#include <Arduino.h>
#include "SensorManager.h"
#include "LedController.h"
#include "KalmanFilter.h"
#include "UbidotsManager.h"

// Configuración WiFi y Ubidots (variables de entorno)
// Para configurar las variables de entorno, crear un archivo .env en la raíz del proyecto
// con las siguientes variables: WIFI_SSID, WIFI_PASSWORD, UBIDOTS_TOKEN, DEVICE_LABEL, TEMP_LABEL, HUM_LABEL
// #define XSTR(x) #x
// #define STR(x) XSTR(x)

// Acceder a las variables de entorno como cadenas
// const char *WIFI_SSID = STR(WIFI_SSID);
// const char *WIFI_PASSWORD = STR(WIFI_PASSWORD);
// const char *UBIDOTS_TOKEN = STR(UBIDOTS_TOKEN);
// const char *DEVICE_LABEL = STR(DEVICE_LABEL);
// const char *TEMP_LABEL = STR(TEMP_LABEL);
// const char *HUM_LABEL = STR(HUM_LABEL);

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
KalmanFilter kalmanTemp(0.1, 0.1, 0.01, 0);
KalmanFilter kalmanHum(0.1, 0.1, 0.01, 0);
UbidotsManager ubidots(WIFI_SSID, WIFI_PASSWORD, UBIDOTS_TOKEN, DEVICE_LABEL, TEMP_LABEL, HUM_LABEL);

void setup()
{
  Serial.begin(115200);
  sensor.begin();
  ledRojo.begin();
  ledAzul.begin();
  ubidots.connectWiFi();
}

void loop()
{
  // Obtener lecturas del sensor
  float temperatura = kalmanTemp.update(sensor.getTemperature());
  float humedad = kalmanHum.update(sensor.getHumidity());

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

  // se  envia la data  ubidotss
  ubidots.sendToUbidots(temperatura, humedad);

  delay(600000); // Esperamos 10 minutos para subir a ubidots (600,000 ms)
}
