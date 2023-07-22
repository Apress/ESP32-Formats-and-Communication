/*******************************************************************************
 * Sketch name: app to transmit and receive data
 * Description: Return direction texyt and double the number
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    12 - Control apps
 ******************************************************************************/

#include <WiFi.h>            // include WiFi library
WiFiClient client;          // associate client and server with
WiFiServer server(80);        //     WiFiClient and WiFiServer libraries
#include <ssid_password.h>        // file with SSID and password
String str, reply, value;
int indexS, indexF, valueN;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  WiFi.begin(ssid, password);       // initialise and connect Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());     // display server IP address
  server.begin();           // initiate server
}

void loop()
{
  client = server.available();
  if (client)           // initialise client connection
  {
    while (!client.available()) {};     // no client request, do nothing
    str = client.readStringUntil('\r');   // map str to message
    Serial.println(str);        // display HTTP GET request
    indexS = str.indexOf("value");      // position of "value" in string
    indexF = str.indexOf("HTTP");
    value = str.substring(indexS+6, indexF-1);  // value=NNN HTTP
    valueN = 2*(value.toInt());     // transform received value
    reply = "left";
    if(str.indexOf("R") != -1) reply = "right"; // str contains "R"
    reply = reply +","+ String(valueN);
    client.println("HTTP/1.1 200 OK");    // HTTP header & response code
    client.println("Content-type:text/html\n"); // \n to generate blank line
    client.println(reply);        // transmit HTTP response
    client.stop();          // close connection
  }
}
