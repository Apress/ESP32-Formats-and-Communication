/*******************************************************************************
 * Sketch name: Transmit message to receiving ESP32
 * Description: Table 8-1
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    8 - WebSocket, remote access and OTA
 ******************************************************************************/

#include <ArduinoWebsockets.h>
using namespace websockets;
WebsocketsClient client;
char ssid[ ] = "login";
char password[ ] = "pass1234";
String TXmsg = "WebSocket message ";
unsigned long lag;
int count = 0;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("softAP address ");
  Serial.println(WiFi.softAPIP());
  client.connect("192.168.4.1", 81, "/");
  client.onMessage(wsEvent);
}

void wsEvent (WebsocketsMessage RXmsg)
{
  Serial.printf("callback %s \n", RXmsg.data());
}

void loop()
{
  if(client.available()) client.poll();
  if(millis() - lag > 2000)
  {
    count++;
    client.send(TXmsg + String(count));
    lag = millis();
  }
}
