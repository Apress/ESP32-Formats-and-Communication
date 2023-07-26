/*******************************************************************************
 * Sketch name: Display black and white image on LCD screen
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include "image.h"          // tab with image data array
#include <TFT_eSPI.h>         // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();        // associate tft with library
int imageW = 64, imageH = 128;      // image width and height

void setup()
{
  tft.init();           // initialise screen
}

void loop()
{
  for (int i=0; i<4; i++)       // for four rotation settings
  {
    tft.fillScreen(TFT_BLACK);      // clear OLED display
    tft.setRotation(i);         // 0 & 2 landscape 1 & 3 portrait
    tft.drawBitmap(0, 0, test, imageW, imageH, TFT_WHITE);  // draw image
    delay(3000);            // delay between image rotations
  }
}
