/*******************************************************************************
 * Sketch name: Display images on webpage
 * Description: Bulb on and off images
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <WebServer.h>          // WebServer library for ESP32
WebServer server(80);
#include <ssid_password.h>
#include "buildpage.h"          // HTML and JavaScript code

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  WiFi.begin(ssid, password);       // initialise and connect Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP Address ");
  Serial.println(WiFi.localIP());     // display ESP32 IP address
  server.begin();
  server.on("/", base);         // function for default webpage
}
void base()             // function to return HTML code
{
  server.send(200, "text/html", page);
}
void loop()
{
  server.handleClient();          // manage HTTP requests
}
