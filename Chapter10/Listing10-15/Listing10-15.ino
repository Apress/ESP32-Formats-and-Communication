/*******************************************************************************
 * Sketch name: Transparent Sprite
 * Description: Transparent ring on colour screen halves
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <TFT_eSPI.h>          // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();        // associate tft with library
TFT_eSprite img = TFT_eSprite(&tft);    // Sprite image pointer to tft

void setup()
{
  tft.init();           // initialise LCD screen
  tft.setRotation(1);         //rotation 1 = USB-C on RHS
  tft.fillRect(0, 0, 120, 135, TFT_GREEN);  // rectangle over half the screen
  tft.fillRect(120, 0, 120, 135, TFT_BLUE);
  img.createSprite(40, 40);
  img.fillCircle(20, 20, 20, TFT_RED);  // centre (x, y), relative to Sprite, radius
  img.fillCircle(20, 20, 15, TFT_BLACK);    // smaller circle
  img.pushSprite(100, 50, TFT_BLACK);   // position relative to screen
}

void loop()             // nothing in loop function
{}
