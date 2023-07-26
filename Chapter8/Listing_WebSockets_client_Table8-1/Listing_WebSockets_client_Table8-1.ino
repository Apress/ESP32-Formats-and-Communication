/*******************************************************************************
 * Sketch name: Transmit message to receiving ESP32
 * Description: Table 8-1
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    8 - WebSocket, remote access and OTA
 ******************************************************************************/

#include <WebSocketsClient.h>
WebSocketsClient websocket;
char ssid[ ] = "login";
char password[ ] = "pass1234";
String TXmsg = "WebSocket message ";
unsigned long lag;
int count = 0;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("softAP Address ");
  Serial.println(WiFi.softAPIP());
  websocket.begin("192.168.4.1", 81, "/");
  websocket.onEvent(wsEvent);
}

void wsEvent(WStype_t type, uint8_t * RXmsg, size_t length)
{
  if(type == WStype_TEXT) Serial.printf("callback \"%s\" \n", RXmsg);
}

void loop()
{
  websocket.loop();
  if(millis() - lag > 2000)
  {
    count++;
    websocket.sendTXT(TXmsg +String(count) + " abc");
    lag = millis();
  }
}
