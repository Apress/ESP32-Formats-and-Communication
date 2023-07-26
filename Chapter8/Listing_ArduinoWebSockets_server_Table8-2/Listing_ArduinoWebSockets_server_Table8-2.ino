/*******************************************************************************
 * Sketch name: Receive message from transmitting ESP32
 * Description: Table 8-2
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    8 - WebSocket, remote access and OTA
 ******************************************************************************/

#include <ArduinoWebsockets.h>
using namespace websockets;
WebsocketsServer server;
WebsocketsClient client;
char ssid[ ] = "login";
char password[ ] = "pass1234";
String str;

void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.print("softAP address ");
  Serial.println(WiFi.softAPIP());
  server.listen(81);
}

void loop()
{
  if(server.poll()) client = server.accept();
  if(client.available())
  {
    WebsocketsMessage RXmsg = client.readBlocking();
    Serial.printf("RXmsg \"%s\" \n", RXmsg.c_str());
    str = "len " + String(RXmsg.length());
    client.send(str);
  }
}
