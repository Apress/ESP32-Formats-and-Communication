/*******************************************************************************
 * Sketch name: Display image with bitmap
 * Description: tick symbol is very small
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

const unsigned char tick [] PROGMEM = {    // store image bitmap
0x01, 0x03, 0x06, 0x2C, 0x38, 0x10, 0x00    //     in PROGMEM
};
#include <TFT_eSPI.h>         // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();        // associate tft with library
int imageW = 6, imageH = 7;       // image height and width

void setup()
{
  tft.init();           // initialise screen
  tft.fillScreen(TFT_BLACK);        // screen background colour
  tft.drawBitmap(20, 20, tick, imageW, imageH, TFT_WHITE);  // draw image
}                    // image width and height

void loop()             // nothing in loop function
{}
