/*******************************************************************************
 * Sketch name: Alternative text and font sizes
 * Description: Different fonts displayed on TTGO T-Watch V2
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

#define LILYGO_WATCH_2020_V2        // define T-watch model
#include <LilyGoWatch.h>        // include library
TTGOClass * ttgo;           // associate objects with libraries
TFT_eSPI * tft;           // graphics library

void setup()
{
  ttgo = TTGOClass::getWatch();
  ttgo->begin();            // initialise ttgo object
  ttgo->openBL();           // turn on backlight
  tft = ttgo->tft;          // shorthand for object
  ttgo->bl->adjust(64);         // reduce brightness from 255
  tft->fillScreen(TFT_BLACK);       // screen background colour
  tft->setTextSize(1);
  tft->setCursor(0,0);
  tft->println("ABab123");          // too small
  tft->drawString("ABab123", 0, 10, 2);     // minimum readable
  tft->drawString("ABab123", 0, 30, 4);     // recommend characters
  tft->drawString("123",     0, 60, 6);     // digits only
  tft->setCursor(0,110);
  tft->setTextSize(2);
  tft->println("ABab123");          // smaller alternative
  tft->drawString("ABab123", 0, 130, 2);      // thin characters
  tft->drawString("ABab123", 0, 170, 4);      // similar to size 1 font 6
//tft->setTextSize(3);            // square-ish characters
//tft->setTextSize(4);            // poor quality characters
}

void loop()               // nothing in loop function
{}
