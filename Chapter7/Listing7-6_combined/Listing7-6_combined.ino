/*******************************************************************************
 * Sketch name: Listing 7-6 extended to control two devices
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

#include <WebServer.h>          // include webserver library
WebServer server (80);          // associate server with library
#include "buildpage.h"          // webpage HTML code
#include <ssid_password.h>        // Wi-Fi SSID and password
int LEDG = 0, LEDR, LEDGpin = 13, LEDRpin = 12;  // define LED pins
String str;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(LEDGpin, OUTPUT);        // define LED pins as output
  pinMode(LEDRpin, OUTPUT);
  WiFi.begin(ssidEXT, password);       // connect and initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP address ");
  Serial.println(WiFi.localIP());     // display ESP32 IP address
  server.begin();           // initialise server
  server.on("/", base);         // load default webpage
  server.on("/LEDGurl", LEDGfunct);      // map URLs to functions
  server.on("/LEDRurl", LEDRfunct);
}

void base()           // function to transmit HTML code
{             //     for default webpage to client
  server.send(200, "text/html", page);
}

void LEDGfunct()          // function to turn on or off green LED
{
  LEDG = 1 - LEDG;
  digitalWrite(LEDGpin, LEDG);       // update LED state
  str = (LEDG == HIGH ? "ON" : "OFF");    // set str to LED state
  server.send(200, "text/plain", str);    // transmit LED state to client
}

void LEDRfunct()            // similar function for red LED
{
  LEDR = !digitalRead(LEDRpin);
  digitalWrite(LEDRpin, LEDR);
  str = (LEDR == HIGH ? "ON" : "OFF");
  server.send(200, "text/plain", str);
}

void loop()
{
  server.handleClient();        // manage XML HTTP requests
//  delay(2);
}
