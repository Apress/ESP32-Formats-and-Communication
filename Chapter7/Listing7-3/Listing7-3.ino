/*******************************************************************************
 * Sketch name: HTTP request
 * Description: Control device from webpage
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

#include <WebServer.h>          // include webserver library
WebServer server (80);          // associate server with library
#include <ssid_password.h>        // Wi-Fi SSID and password file
int LEDstate = 0, LEDpin = 12;      // LED state and GPIO pin

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(LEDpin, OUTPUT);        // define LED pin as output
  WiFi.begin(ssid, password);       // connect and initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());     // display ESP32 IP address
  server.begin();           // initialise server
  server.on("/", editHTML);       // load default webpage
  server.on("/LEDurl", LEDfunct);     // map URL to function
}

void LEDfunct()           // function mapped to URL
{
  LEDstate = 1 - LEDstate;
  digitalWrite(LEDpin, LEDstate);     // update LED state
  editHTML();           // call function to update HTML
}

void editHTML()
{
  String page = R"(
  <!DOCTYPE html><html><head>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <meta charset='UTF-8'>
  </head>
  <body>
  <button onclick='location.href="/LEDurl";'>LED</button>
  <p>LEDlabel</p>
  </body></html>
  )";
  page.replace("LEDlabel", LEDstate ? " ON" : "OFF"); // LEDstate on webpage
  server.send(200, "text/html", page);      // load updated webpage
}
void loop()
{
  server.handleClient();          // manage HTTP requests
  delay(2);               // prevent ESP32 resetting
}
