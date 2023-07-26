/*******************************************************************************
 * Sketch name: Main sketch with esp_http_server
 * Description: Webpage with control functions
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    11 - ESP32_CAM camera
 ******************************************************************************/

#include <WiFi.h>            // include WiFi and
#include <esp_http_server.h>        //     esp_http_server libraries
#include <ssid_password.h>        // file with SSID and password
#include "buildpage.h"          // webpage HTML & AJAX code
int lightPin = 13;
int servoPin = 14;
int channel = 10, freq = 50, resol = 8;
int pulse;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(lightPin, OUTPUT);
  ledcAttachPin(servoPin, channel);     // attach servoPin to channel
  ledcSetup(channel, freq, resol);      // define square wave frequency
  WiFi.begin(ssid, password);       // connect and initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());     // display web server IP address
  startServer();
}

void loop()             // nothing in loop function
{}
