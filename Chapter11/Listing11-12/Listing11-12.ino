/*******************************************************************************
 * Sketch name: Main sketch with WebSocket
 * Description: Webpage with control functions
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

#include <WebServer.h>          // include Webserver library
WebServer server;           // associate server with library
#include <WebSocketsServer.h>       // include Websocket library
WebSocketsServer websocket = WebSocketsServer(81);  // set WebSocket port 81
#include <ssid_password.h>        // file with SSID and password
#include "buildpage.h"          // webpage HTML & AJAX code
int lightPin = 13;
int servoPin = 14;
int channel = 10, freq = 50, resol = 8;
String str, text[3], direct = "null", Ndirect;
int comma1, comma2, light = 0, Nlight, servo = 20, Nservo, duty;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(lightPin, OUTPUT);
  ledcAttachPin(servoPin, channel);     // attach servoPin to channel
  ledcSetup(channel, freq, resol);      // define square wave frequency
  WiFi.begin(ssid, password);       // connect and initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());     // display web server IP address
  server.begin();
  server.on("/", base);         // load default webpage
  websocket.begin();          // initialise WebSocket
  websocket.onEvent(wsEvent);       // wsEvent on WebSocket event
}

void base()             // function to return HTML code
{
  server.send(200, "text/html", page);
}

void loop()
{
  server.handleClient();        // manage HTTP requests
  websocket.loop();         // handle WebSocket data
}
