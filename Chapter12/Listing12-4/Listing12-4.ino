/*******************************************************************************
 * Sketch name: app with image streaming and control functions
 * Description: Combination of Listings 12-3 and 11-7
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    12 - Control apps
 ******************************************************************************/

#include <WiFi.h>
WiFiClient client;
WiFiServer server(80);
#include <esp_camera.h>
#include <esp_http_server.h>
#include <ssid_password.h>
//char ssidAP[] = "ESP32CAM";
//char passwordAP[] = "pass1234";

#include "stream_handler.h"
String str, reply, direct;
String frames[] = {"96x96","QQVGA 160x120","QCIF 176x144","HQVGA 240x176",
                   "240x240","QVGA 320x240","CIF 400x296","HVGA 480x320",
                   "VGA 640x480","SVGA 800x600","XGA 1024x768","HD 1280x720",
                   "SXGA 1280x1024","UXGA 1600x1200"};
int indexS, indexF, frm, oldfrm = 0;

void setup()
{
  Serial.begin(115200);
//  WiFi.softAP(ssidAP, passwordAP);
//  Serial.println(WiFi.softAPIP());
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());
  configCamera();           // functions to configure camera
  startServer();            //     and start server
  server.begin();
}

void loop()
{
  client = server.available();
  if (client)
  {
    while (!client.available()) {};  // do nothing
    str = client.readStringUntil('\r');
    Serial.println(str);
    indexS = str.indexOf("frame");
    indexF = str.indexOf("HTTP");
    frm = (str.substring(indexS+6, indexF-1)).toInt();
    if(frm != oldfrm)
    {
      oldfrm = frm;
      sensor_t * s = esp_camera_sensor_get();
      s->set_framesize(s, (framesize_t)frm);    // update image size
      Serial.printf("image size %d \n", frm);   // frm = 0 to 13
    }
    direct = "left";
    if(str.indexOf("R") != -1) direct = "right";
    else if(str.indexOf("F") != -1) direct = "forward";
    else if(str.indexOf("B") != -1) direct = "backward";
    else if(str.indexOf("S") != -1) direct = "stop"; 
    reply = direct +","+ frames[frm];
    client.println("HTTP/1.1 200 OK");  // HTTP header starts with response code
    client.println("Content-type:text/html\n");  // \n required for to generate blank line as part response to client
    client.println(reply); // server response
    client.stop();  // close connection
  }
}
