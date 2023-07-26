/*******************************************************************************
 * Sketch name: Main sketch for streaming camera images
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

#include <WiFi.h>            // include libraries
#include <esp_camera.h>
#include <esp_http_server.h>
#include <ssid_password.h>        // file with SSID and password
#include "buildpage.h"          // webpage HTML & AJAX code
#include "stream_handler.h"       // stream handler function

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  WiFi.begin(ssid, password);       // initialise and connect Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());     // display server IP address
  configCamera();           // functions to configure camera
  startServer();            //     and start server
}

void loop()             // nothing in loop function
{}
