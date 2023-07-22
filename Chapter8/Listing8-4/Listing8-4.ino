/*******************************************************************************
 * Sketch name: WebSerial webpage
 * Description: Display data and receive text from Serial Monitor
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    8 - WebSocket, remote access and OTA
 ******************************************************************************/

#include <WebSerial.h>          // include WebSerial library and
AsyncWebServer server(80);        //     associate server with library
char ssidAP[] = "ESP32_WebSerial";      // microcontroller as access point
char passwordAP[] = "pass1234";
int count = 0;            // with the required channel

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  WiFi.softAP(ssidAP, passwordAP);      // connect to softAP device
  Serial.println(WiFi.softAPIP());      // display softAP address
  WebSerial.begin(&server);       // initialise WebSerial
  server.begin();           //     and AsyncWebServer
  WebSerial.msgCallback(TypeHere);
}

void TypeHere(uint8_t *text, size_t length)
{
  Serial.print("Text: ");
  for(int i=0; i<length; i++) Serial.print((char)text[i]);
  Serial.println();
}

void loop()
{
  Serial.println(count);        // display to Serial Monitor
  WebSerial.println(count);       // and to WebSerial webpage
  count++;
  delay(1000);
}
