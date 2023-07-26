/*******************************************************************************
 * Sketch name: Display additional font
 * Description: HEX data of font in Listing 15-1
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

#include <TFT_eSPI.h>          // include TFT_eSPI libary
#include "newfont.h"          // tab containing Listing 15-1
TFT_eSPI tft = TFT_eSPI();
String str = "ABC abc 123 $@. The quick brown fox jumps over the lazy dog";

void setup()
{
  Serial.begin(115200);
  tft.begin();            // init() and begin() are equivalent
  tft.setFreeFont(&Rock_Salt_Regular_24);   // define additional font
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(0,30);
  tft.print(str);           // display text with wrap around
  delay(5000);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0,30);
  splitText(str);           // display parsed text
}

void splitText(String text)       // see Listing 2-7

void loop()             // nothing in loop function
{}
