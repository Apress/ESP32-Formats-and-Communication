/*******************************************************************************
 * Sketch name: Load webpage with HTML code in Gzipped HEX format
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <WebServer.h>          // include WebServer library
WebServer server(80);         // associate server with library
#include <ssid_password.h>
#include "buildpage.h"          // HTML code

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  WiFi.begin(ssid, password);       // initialise and connect Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());
  server.begin();
  server.on("/", base);         // function for default webpage
}

void base()
{
//  server.send_P(200, "text/html", page);    // send HTML code
  server.sendHeader("content-encoding", "gzip");  // file in GZIP format
  server.send_P(200, "text/html", htmlHEX, sizeof(htmlHEX));  // send file
//  server.send_P(200, "text/html", htmlHEX, fileLength);   // send file
}

void loop()
{
  server.handleClient();          // manage HTTP requests
}
