#include <HTTPClient.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFS.h>
#include <DHTesp.h>

#include "config.h"
#include "Server.hpp"
#include "ESP32_Utils.hpp"

//Declaracion de pines para sensores
#define sensorSuelo 32//Sensor de humedad del suelo
const int valvula = 2; //Valvula de riego
const int sensorAire = 18; //Sensor de humedad y temperatura del aire
const int caudal = 15; //Caudalimetro
DHTesp dhtSensor; //Sensor DHT
//Variables utilizadas por el caudalimetro
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 7.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
//Metodo para contar pulsos del caudalimetro
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setup(void)
{
  Serial.begin(115200);
  pinMode(valvula, OUTPUT);
  pinMode(sensorSuelo, INPUT);
  //Configuracion caudalimetro
  pinMode(caudal, INPUT_PULLUP);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  attachInterrupt(digitalPinToInterrupt(caudal), pulseCounter, FALLING);
  //Configuracion sensor DHT
  dhtSensor.setup(sensorAire, DHTesp::DHT22);
  
  SPIFFS.begin(true);
  ConnectWiFi_STA();

  InitServer();
}

void loop(void)
{
  //Sensor de Temperatura y humedad del aire
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  float temp = data.temperature; //Lectura de temperatura
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println("°C");
  float humid = data.humidity; //Lectura de humedad del aire
  Serial.print("Humedad del aire: ");
  Serial.print(humid);
  Serial.println("%");
  //Sensor de humedad del suelo
  int sensorValue = analogRead(sensorSuelo);
  float humidity = map(sensorValue, 900, 4095, 100, 0); //Conversion a porcentaje
  Serial.print("Humedad del suelo: ");
  Serial.print(humidity);
  Serial.println("%");
  //Caudalimetro
  currentMillis = millis();
  //Realizar la medicion cuando se cumpla el intervalo
  if (currentMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    // Flujo de agua en litros por minuto
    Serial.print("Flujo de agua: ");
    Serial.print(int(flowRate));
    Serial.print("L/min");
    Serial.print("\t");
    // Litros totales utilizados
    Serial.print("Total de agua utiliada (Mililitros y litros): ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");
  }
  //Envio de datos al servidor de hosting
  HTTPClient http;
  String datos = "temp="+String(temp)+"&humid="+String(humid)+"&ground="+String(humidity); //Juntar datos
  http.begin("https://tec2riego.000webhostapp.com/CodigoPhp/sensores.php"); //URL de envio
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int respuesta = http.POST(datos); //POST de los datos
  String response = http.getString();
  Serial.print("Respuesta: ");
  Serial.print(respuesta);
  Serial.println(response);
  
  delay(2000);
}
