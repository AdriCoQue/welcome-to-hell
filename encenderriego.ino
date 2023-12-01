#include <HTTPClient.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFS.h>
#include <DHTesp.h>

#include "config.h"
#include "Server.hpp"
#include "ESP32_Utils.hpp"

const int led = 2;
const int sensorTemp = 16;
const int sensorSuelo = 4;
DHTesp dhtSensor;

void setup(void)
{
  pinMode(led, OUTPUT);
  pinMode(sensorSuelo, INPUT);
  Serial.begin(115200);
  SPIFFS.begin(true);
  dhtSensor.setup(sensorTemp, DHTesp::DHT22);
  
  ConnectWiFi_STA();

  InitServer();
}

void loop(void)
{
  //Humedad del aire y temperatura
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  //Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  //Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  //Serial.println("---");
  //Humedad del suelo
  float humedadsuelo;
  humedadsuelo = analogRead(sensorSuelo);
  humedadsuelo=(humedadsuelo/4095)*100;
  humedadsuelo=(humedadsuelo-100)*-1;
  //Serial.print("Humedad del Suelo: ");
  //Serial.print(humedadsuelo);
  //Serial.print("%");
  //Serial.println(); 
  HTTPClient http;
  String temp = String(data.temperature,2);
  String humid = String(data.temperature,1);
  String ground = String(humedadsuelo);
  //String temp = "27";
  //String humid = "25";
  //String ground = "46";
  String datos = "temp="+temp+"&humid="+humid+"&ground="+ground;
  //http.begin("http://10.8.9.104/sensores.php?temp=26&humid=23&ground=45");
  http.begin("http://10.8.9.104/sensores.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //http.addHeader("Content-Type", "plain-text");
  //int respuesta = http.GET();
  int respuesta = http.POST(datos);
  String response = http.getString();
  Serial.print("Respuesta: ");
  Serial.print(respuesta);
  Serial.println(response);
  
  delay(10000);
  
}
