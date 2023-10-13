#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFS.h>

#include "config.h"
#include "Server.hpp"
#include "ESP32_Utils.hpp"

const int led = 2;

void setup(void)
{
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  SPIFFS.begin(true);
  
  ConnectWiFi_STA();

  InitServer();
}

void loop(void)
{
}
