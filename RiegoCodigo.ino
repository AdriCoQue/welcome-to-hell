/**
   ESP32 + DHT22 Example for Wokwi
   
   https://wokwi.com/arduino/projects/322410731508073042
*/
#include <WiFi.h>
#include "DHTesp.h"

// DHT
const int DHT_PIN = 26;
const int SensorAGRO = 25;
//Es para el Relay
const int RELAY_PIN = 2;

//int ARDUINO_PIN = 5;


//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

//---------------------Credenciales de WiFi-----------------------

const char* ssid     = "R ITCHII Wi-Fi";
const char* password = "";

//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;

String header; // Variable para guardar el HTTP request

String estadoSalida = "off";

//------------------------CODIGO HTML------------------------------


String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<title>ITCHII</title>"
"<style>"
"body {  margin: 0%;        padding: 0%;     width:100%;   height:100%;   background: linear-gradient(90deg, rgba(56,121,9,1) 0%, rgba(23,91,31,1) 50%, rgba(56,121,9,1) 100%); }rgb(0, 0, 0)"
"h1 {  color: rgb(255, 255, 255); }"
"button{        width: 30%;        height: 100%;        background-color:black;        color: beige;        border: 3px solid;        border-radius:1vw;        border-color: rgba(245, 245, 220, 0.438);        font-family:Verdana, Geneva, Tahoma, sans-serif;        font-size:1.5vw;    }"
"a,p,h1{        width: 100%;        height: 100%;    }"
"#conPrincipal{        margin-top: 5%;        width: 40%;        height: 80%;        background-color: rgba(59, 59, 59, 0.363);        border: 3px solid;        border-radius:1vw;        box-shadow:  2px 2px 20px rgb(3, 36, 41);    }"
"#conS{        width: 100%;        height: 100%;    }"
"#conS2{        width: 80%;        height: 100%;        text-align: left;    }"
"</style>"
"</head>"
"<body>"
"<center>"
        "<div id='conPrincipal'>"
            "<div id='conS'>"
                "<h1>Sistema de Riego ITCHII</h1>"
            "</div>"
            "<div id='conS'>"
                "<p><a href='/on'><button>Regar</button></a></p>"
            "</div>"
            "<div id='conS'>"
                "<p><a href='/off'><button>Suspender</button></a></p>"
            "</div>"
        "</div>"
    "</center>"

    "<center>"
        "<div id='conPrincipal'>"
            "<div id='conS'>"
                "<h1>Señales</h1>"
            "</div>"
            "<div id='conS2'>"
                "<p>Humedad de aire: </p>"
            "</div>"
            "<div id='conS2'>"
                "<p>Humedad de suelo: </p>"
            "</div>"
            "<div id='conS2'>"
                "<p>Temperatura: </p>"
            "</div>"
        "</div>"
    "</center>"

"</body>"
"</html>";




//Libreria de DHT
DHTesp dhtSensor;

void setup() {





  
  //Desconocido 
  Serial.begin(115200);
  //DHCT
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  pinMode(RELAY_PIN, OUTPUT);
/////////////////////////////////////////////////////////////////////////
  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      //IPAddress ip(192,168,1,180); 
      //IPAddress gateway(192,168,1,1); 
      //IPAddress subnet(255,255,255,0); 
      //WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
      server.begin(); // iniciamos el servidor
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

///////////////////////////////////////////////////////

void loop() {
   TempAndHumidity  data = dhtSensor.getTempAndHumidity();
   Serial.println("Temp: " + String(data.temperature, 2) + "°C");
   Serial.println("Humidity: " + String(data.humidity, 1) + "%");
   Serial.println("---");
   
   
    // put your main code here, to run repeatedly:
  float humedadsuelo;
  humedadsuelo = analogRead(SensorAGRO);
  humedadsuelo=(humedadsuelo/1023)*100;
  humedadsuelo=(humedadsuelo-100)*-1;
  Serial.print("Humedad del Suelo: ");
  Serial.print(humedadsuelo);
  Serial.print("%");
  Serial.println();   

  
 WiFiClient client = server.available();   // Escucha a los clientes entrantes
      
  if (client) {   
    Serial.println(digitalRead(RELAY_PIN));
    
    
                              // Si se conecta un nuevo cliente
    Serial.println("New Client.");          // 
    String currentLine = "";                //
    while (client.connected()) { 


  

   
 // float sensorValue = analogRead(SensorAGRO);
 // Serial.println(sensorValue);
  //delay(1000);




    if(digitalRead(RELAY_PIN) == 0 || digitalRead(RELAY_PIN) == 1 ){   
      Serial.println(digitalRead(RELAY_PIN));
      }
      
      
      // loop mientras el cliente está conectado
      if (client.available()) {             // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        //Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del 
          // HTTP request del cliente, entonces respondemos:
          if (currentLine.length() == 0) {
            //client.println("HTTP/1.1 200 OK");
            //client.println("Content-type:text/html");
            //client.println("Connection: close");
            //client.println();
            
            // enciende y apaga el GPIO
            if (header.indexOf("GET /on") >= 0) {
              //Serial.println("GPIO on");
              estadoSalida = "on";
              digitalWrite(RELAY_PIN,HIGH);
             // digitalWrite(RELAY_PIN, HIGH);           

            } else if (header.indexOf("GET /off") >= 0) {
              //Serial.println("GPIO off");
              estadoSalida = "off";
              digitalWrite(RELAY_PIN, LOW);
            }
            
            // Muestra la página web
            client.println(pagina);
            
            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      
      
      
      
      }

          // TempAndHumidity  data = dhtSensor.getTempAndHumidity();
          // Serial.println("Temp: " + String(data.temperature, 2) + "°C");
          // Serial.println("Humidity: " + String(data.humidity, 1) + "%");
          // Serial.println("---");

          // while(data.temperature >= 14 && data.temperature <= 30 && data.humidity <= 60.0){
          // digitalWrite(RELAY_PIN, HIGH);
          // delay(500);
          
          // TempAndHumidity  data = dhtSensor.getTempAndHumidity();

          // Serial.println("Temp: " + String(data.temperature, 2) + "°C");
          // Serial.println("Humidity: " + String(data.humidity, 1) + "%");
          // Serial.println("---");

          // if(data.humidity >= 80 || data.temperature <= 14){
          //   break;
          //   }
          // }
          // digitalWrite(RELAY_PIN, LOW);
          // delay(500);

          // delay(1000);
            
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }














  // delay(20000);



  // TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  // Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  // Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  // Serial.println("---");

  // while(data.temperature >= 14 && data.temperature <= 30 && data.humidity <= 60.0){
  // digitalWrite(RELAY_PIN, HIGH);
  // delay(500);
  
  // TempAndHumidity  data = dhtSensor.getTempAndHumidity();

  // Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  // Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  // Serial.println("---");

  // if(data.humidity >= 80 || data.temperature <= 14){
  //   break;
  //   }
  // }
  // digitalWrite(RELAY_PIN, LOW);
  // delay(500);

  // delay(1000);
}