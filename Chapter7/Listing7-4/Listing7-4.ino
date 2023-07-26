/*******************************************************************************
 * Sketch name: XML HTTP request
 * Description: Control device from webpage
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

#include <WebServer.h>
WebServer server (80);
#include <ssid_password.h>
#include "buildpage.h"          // HTML code for webpage
int LEDstate = 0, LEDpin = 12;
String str;

void setup()
{
  Serial.begin(115200);
  pinMode(LEDpin, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());
  server.begin();
  server.on("/", base);         // load default webpage
  server.on("/LEDurl", LEDfunct);
}

void base()           // function to transmit HTML code
{             //     for default webpage to client
  server.send(200, "text/html", page);
}

void LEDfunct()
{
  LEDstate = 1 - LEDstate;
  digitalWrite(LEDpin, LEDstate);
  str = (LEDstate == HIGH ? "ON" : "OFF");  // map LEDstate to ON or OFF
  server.send(200, "text/plain", str);    // server response to 
}               //    client XML HTTP request

void loop()
{
  server.handleClient();
  delay(2);
}
