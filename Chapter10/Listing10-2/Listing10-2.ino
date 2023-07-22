/*******************************************************************************
 * Sketch name: Display black and white image on OLED screen
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include "image.h"          // tab with image data array
#include <Adafruit_SSD1306.h>       // include libraries
int oledW = 128, oledH = 64;        // OLED width and height
int imageW = 64, imageH = 128;      // image width and height
Adafruit_SSD1306 oled(oledW, oledH, &Wire, -1);

void setup()
{
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // initialise OLED
}

void loop()
{
  for (int i=0; i<4; i++)       // for four rotation settings
  {
    oled.clearDisplay();        // clear OLED display
    oled.setRotation(i);        // 0 & 2 landscape 1 & 3 portrait
    oled.drawBitmap(0, 0, test, imageW, imageH, WHITE); // draw image
    oled.display();
    delay(3000);            // delay between image rotations
  }
}
