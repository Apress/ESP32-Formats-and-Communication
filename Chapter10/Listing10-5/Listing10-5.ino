/*******************************************************************************
 * Sketch name: Display colour image on LCD screen
 * Description: Similar to Listing 10-4
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include "image.h"        // tab with image data array
#include <TFT_eSPI.h>       // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();      // associate tft with library
int imageW = 135, imageH = 240;   // image width and height

void setup()
{
  tft.init();         // initialise screen
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);     // required to retain image colour
  tft.pushImage(50, 0, imageW, imageH, test); // draw image
//  tft.drawBitmap(0, 0, test, imageW, imageH, TFT_WHITE);
}

void loop()           // nothing in loop function
{}
