#include <Arduino.h>
#include "SensorManager.h"
#include "LedController.h"
#include "KalmanFilter.h"
#include "UbidotsManager.h"
#include "JsonManager.h"
#include "MQTTManager.h"

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
#define LDR_PIN 16
#define LED_LDR_PIN 18

// Umbrales de temperatura y humedad
const int TEMPERATURA_UMBRAL = 30;
const int HUMEDAD_MINIMA = 40;
const int HUMEDAD_MAXIMA = 60;

// Intervalos de tiempo
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 300000; // 5 minutos

// Instancias de las clases
SensorManager sensor(DHT_PIN, DHT_TYPE);
LedController ledRojo(LED_ROJO_PIN);
LedController ledAzul(LED_AZUL_PIN);
LedController ledLDR(LED_LDR_PIN);
KalmanFilter kalmanTemp(0.1, 0.1, 0.01, 0);
KalmanFilter kalmanHum(0.1, 0.1, 0.01, 0);
UbidotsManager ubidots(WIFI_SSID, WIFI_PASSWORD, UBIDOTS_TOKEN, DEVICE_LABEL, TEMP_LABEL, HUM_LABEL, LIGHT_LABEL);
MQTTManager mqttManager;
JsonManager jsonManager;

void setup()
{
  Serial.begin(115200);
  sensor.begin();
  ledRojo.begin();
  ledAzul.begin();
  ledLDR.begin();
  pinMode(LDR_PIN, INPUT);
  ubidots.connectWiFi();
  mqttManager.init();
}

void loop()
{
  // Mantener la conexión MQTT
  if (!mqttManager.isConnected())
  {
    mqttManager.reconnect();
  }
  mqttManager.loop();

  // Obtener lecturas del sensor, solo cada 1 segundo
  static unsigned long lastReadTime = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastReadTime >= 1000)
  { // Esperar 1 segundo entre lecturas
    lastReadTime = currentMillis;

    // Obtener lecturas del sensor
    float temperatura = kalmanTemp.update(sensor.getTemperature());
    float humedad = kalmanHum.update(sensor.getHumidity());

    // Lectura del estado del sensor LDR
    int ldrState = digitalRead(LDR_PIN);

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

    // Mostrar estado del sensor LDR
    Serial.print("Estado del LDR: ");
    Serial.println(ldrState == LOW ? "Oscuridad" : "Luz detectada");

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

    // Control del LED asociado al LDR
    if (ldrState == LOW) // Oscuridad detectada
    {
      ledLDR.turnOn();
    }
    else
    {
      ledLDR.turnOff();
    }

    if (currentMillis - lastSendTime >= sendInterval)
    {
      // Enviar datos
      String mqttPayload = jsonManager.createJson(temperatura, humedad, ldrState);
      mqttManager.publish(mqttPayload);
      ubidots.sendToUbidots(temperatura, humedad, ldrState);

      // Actualizar el tiempo de envío
      lastSendTime = currentMillis;
    }
  }
}
