#include <WiFi.h>
 
const char* ssid     = "R ITCHII Wi-Fi";
const char* password = "";
const char* hostname = "riego";

//IPAddress ip(192, 168, 1, 200);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 255, 0);
 
void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
 
  WiFi.mode(WIFI_STA);
  //WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a:\t");
  Serial.println(ssid); 

  // Esperar a que nos conectemos
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200); 
  Serial.print('.');
  }

  // Mostrar mensaje de exito y dirección IP asignada
  Serial.println("Conexión establecida");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}
 
void loop() 
{

}