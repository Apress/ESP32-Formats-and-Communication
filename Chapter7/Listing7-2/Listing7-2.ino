/*******************************************************************************
 * Sketch name: QR code generation
 * Description: Display QR code on M5Stack Core2
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

#include <M5Core2.h>
String url;

void setup()
{
  M5.begin();            // display M5Stack QR code
  M5.Lcd.qrcode("http://www.m5stack.com", 0, 0, 200, 3);
}

void loop()
{
  if(Serial.available())        // URL entered on Serial Monitor
  {
    url = Serial.readString();
    url = "http://" + url;        // include http:// prefix
    M5.Lcd.qrcode(url, 0, 0, 200, 3);   // display QR code version 3
  }
}
