/*******************************************************************************
 * Sketch name: Required instructions
 * Description: Set up libraries and watch functions
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
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
  tft->setTextColor(TFT_WHITE, TFT_BLACK);  // text and background colour
  tft->setTextSize(1);          // text size of 1 to 7
  tft->setCursor(0,0);          // position cursor
  
