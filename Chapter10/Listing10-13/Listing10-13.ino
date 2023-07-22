/*******************************************************************************
 * Sketch name: Display image as sprite
 * Description: Display on 1.3" 240x240 pixel TFT ST7789 LCD 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <TFT_eSPI.h>          // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();        // associate tft with library
TFT_eSprite img = TFT_eSprite(&tft);    // Sprite image pointer to tft
#include "background.h"          // background in RGB565 format
#include "snowflake.h"          // foreground in XBM format
int width = TFT_WIDTH, height = TFT_HEIGHT; // LCD dimensions
int colorDepth = 8;          // 1, 4, 8 or 16-bit colours
const int Nflake = 500;         // const to define array size
int x[Nflake], y[Nflake];
float speed[Nflake];
float dx, dy;

void setup()
{
  tft.init();           // initialise LCD
  img.setSwapBytes(true);       // required to retain image colour
  img.setColorDepth(colorDepth);      // Sprite colour depth
  img.createSprite(width, height);      // full screen Sprite
  for (int i=0; i<Nflake; i++)      // create foreground images
  {
    x[i] = random(width);     // horizontally distributed
    y[i] = random(height/2)-height/2-flakeH;  // foreground image above screen
    speed[i] = random(10,30)/200.0;   // foreground image speed
  }
}

void loop()
{
  img.pushImage(0, 0, width, height, bckgnd);  // add background image to Sprite
  for (int i=0; i<Nflake; i++)      // move foreground images
  {
    dx = random(-5,5);          // random makes image "float"
    dy = 10 + random(-5,5);       // add 10 to make image fall
    x[i] = x[i] + round(dx*speed[i]);
    y[i] = y[i] + round(dy*speed[i]);
    constrain(x[i], 0, width-flakeW); // keep images on screen
                                      // add foreground image to Sprite
    img.drawXBitmap(x[i], y[i], snowflake, flakeW, flakeH, TFT_WHITE);
  }
  img.pushSprite(0, 0);         // display Sprite on LCD screen
  delay(5);
}
