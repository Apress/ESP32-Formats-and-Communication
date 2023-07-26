/*******************************************************************************
 * Sketch name: ESP32 CAM - client
 * Description: Display images on LCD connected to remote ESP32
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

#include <esp_camera.h>          // include camera library and
#include "config_pins.h"        // camera configure pins tab
#include <WebSocketsClient.h>       // WebSocket client library
WebSocketsClient websocket;       // associate websocket with lib
char ssidAP[ ] = "ESP32CAM";      // softAP ssid and password
char passwordAP[ ] = "pass1234";

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssidAP, passwordAP);       // initialise and connect to Wi-Fi
  while(WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("client softAP address ");         // client softAP IP address
  Serial.println(WiFi.localIP());        // softAP IP address
  websocket.begin("192.168.4.1", 81, "/");     // server softAP IP address
  configCamera();           // function to configure camera
}

void loop()
{
  websocket.loop();         // handle WebSocket data
  camera_fb_t * frame = esp_camera_fb_get();  // get image from camera
  websocket.sendBIN((uint8_t *)frame->buf, (size_t)frame->len);
  esp_camera_fb_return(frame);      // return frame buffer to driver
}
