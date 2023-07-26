/*******************************************************************************
 * Sketch name: Display images on webpage when no internet connection
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <WebServer.h>
WebServer server(80);
char ssidAP[] = "ESP32_image";      // access point ssid and password
char passwordAP[] = "pass1234";
#include "buildpage.h"
#include "images.h"         // image data arrays

void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssidAP,passwordAP);     // initialise access point
  Serial.print("softAP address ");
  Serial.println(WiFi.softAPIP());      // display access point IP
  server.begin();
  server.on("/", base);
  server.on("/on", bulbon);       // map functions to URLs
  server.on("/off", bulboff); 
}
void bulbon()     // function to send response with bulb on image data
{
  server.send_P(200, "image/gif", onHEX, sizeof(onHEX));
}
void bulboff()      // function to send response with bulb off image data
{
  server.send_P(200, "image/gif", offHEX, sizeof(offHEX));
}
void base()
{
  server.send(200, "text/html", page);
}
void loop()
{
  server.handleClient();
}
