/*******************************************************************************
 * Sketch name: Receive message from transmitting ESP32
 * Description: Table 8-2
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    8 - WebSocket, remote access and OTA
 ******************************************************************************/

#include <WebSocketsServer.h>
WebSocketsServer websocket = WebSocketsServer(81);
char ssid[ ] = "login";
char password[ ] = "pass1234";
String str;

void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssid,password);
  Serial.print("softAP address ");
  Serial.println(WiFi.softAPIP());
  websocket.begin();
  websocket.onEvent(wsEvent);
}

void wsEvent(uint8_t n, WStype_t type, uint8_t * RXmsg, size_t length)
{
  if(type == WStype_TEXT)
  {
    Serial.printf("RXmsg \"%s\" from device %d \n", RXmsg, n);
    str = "";
    for (int i=0; i<length; i++) if (isDigit(RXmsg[i])) str = str + char(RXmsg[i]);
    websocket.sendTXT(n, "msg rcvd " +str);
  }
}

void loop()
{  
  websocket.loop();
}
