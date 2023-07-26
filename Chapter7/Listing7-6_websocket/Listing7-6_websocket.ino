/*******************************************************************************
 * Sketch name: Listing 7-6 extended with webSockets
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

#include <WebServer.h>          // include webserver library
WebServer server (80);          // associate server with library
#include <WebSocketsServer.h>            // include Websocket library
WebSocketsServer websocket = WebSocketsServer(81);  // set WebSocket port 81
#include "buildpage.h"          // webpage HTML code
#include <ssid_password.h>        // Wi-FI SSID and password
int LEDG, LEDR, LEDpin, LEDGpin = 13, LEDRpin = 12;  // define LED pins
String str, strR, strG, json;

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
  websocket.begin();            // initialise WebSocket
  websocket.onEvent(wsEvent);
}

void wsEvent(uint8_t n, WStype_t type, uint8_t * message, size_t length)
{
  if(type == WStype_TEXT)
  {
    str = "";            // convert message to string
    for (int i=0; i<length; i++) str = str + char(message[i]);
    LEDpin = (str == "LEDG" ? LEDGpin : LEDRpin); // determine required LED
    digitalWrite(LEDpin, !digitalRead(LEDpin));   // turn on or off LED
    readLED();              // call readLED function
  }
}

void readLED()        // function to broadcast LED states to clients
{
  strG = String(digitalRead(LEDGpin));      // determine LED states
  strR = String(digitalRead(LEDRpin));
  strG = (strG == "1" ? "ON" : "OFF");      // convert LED states
  strR = (strR == "1" ? "ON" : "OFF");      //     to "ON" or "OFF"
  json =  "{\"var1\": \"" + strG + "\",";     // partition with comma
  json += " \"var2\": \"" + strR + "\"}";     // end with close bracket
  websocket.broadcastTXT(json.c_str(), json.length());   // broadcast to clients
}

void base()           // function to transmit HTML code
{             //     for default webpage to client
  server.send(200, "text/html", page);
}

void LEDGfunct()          // function to turn on or off green LED
{
  LEDG = !digitalRead(LEDGpin);
  digitalWrite(LEDGpin, LEDG);       // update LED state
  readLED();
  server.send(200, "text/plain", strG);
}

void LEDRfunct()            // similar function for red LED
{
  LEDR = !digitalRead(LEDRpin);
  digitalWrite(LEDRpin, LEDR);
  readLED();
  server.send(200, "text/plain", strR);
}

void loop()
{
  server.handleClient();        // manage XML HTTP requests
  websocket.loop();
}
