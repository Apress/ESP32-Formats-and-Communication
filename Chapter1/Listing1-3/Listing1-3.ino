/*******************************************************************************
 * Sketch name: Digital spirit level
 * Description: Tone function of M5Unified library for M5Stack Core2
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    1 - ESP32 microcontroller
 ******************************************************************************/

#include <M5Unified.h>          // include M5Unified library
#include "image.h"          // include bitmap image
float accX, accY, accZ, degX, degY;
int pitch = 15, roll = 30;  // different tilt thresholds
int x, y;             // for X and Y axes

void setup()
{
  M5.begin();
  M5.Imu.begin();           // initialise accelerometer
  M5.Speaker.begin();         //   and M5Unified speaker
  M5.Speaker.setVolume(80);       // volume range: 0 to 255
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(GREEN, BLACK);
  M5.Lcd.fillScreen(BLACK);     // draw speaker image from image.h file
  M5.Lcd.drawBitmap(240, 10, speaker, 40, 40, WHITE);
  M5.Lcd.drawFastHLine(0, 160, 320, WHITE); // draw horizontal and
  M5.Lcd.drawFastVLine(160, 100, 120, WHITE); // vertical axes for "bubble"
}

void loop()
{
  M5.Imu.getAccel(&accX, &accY, &accZ);   // 3-axis accelerometer data
  degY = atan(accY/accZ) * RAD_TO_DEG;  // roll (tilt left or right)
  degX = asin(accX/sqrt(accX*accX+accY*accY+accZ*accZ));
  degX = degX * RAD_TO_DEG;   // pitch (taking off and landing)
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.print("   roll   pitch");  // switched when USB to left
  M5.Lcd.setCursor(0, 35);
  M5.Lcd.printf("%7.1f %7.1f", degX, degY);
  x = 160*(accX + 1);
  y = 60*(-accY + 1) + 100;       // position of spirit level "bubble"
  M5.Lcd.drawCircle(x, y, 8, GREEN);    // co-ordinates of centre, radius
  if(fabs(degY) > pitch || fabs(degX) > roll)
  {
    M5.Speaker.tone(1000, 50);      // sound freq Hz and time ms
    M5.Lcd.fillRect(0, 60, 320, 40, BLACK); // over-write previous text
    M5.Lcd.setCursor(0, 60);
    M5.Lcd.setTextColor(RED);       // display tilt message
    if(degX > roll) M5.Lcd.print("           right-side high");
    else if(degX < -roll ) M5.Lcd.print("left-side high");
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.setTextColor(YELLOW);
    if(degY > pitch) M5.Lcd.print("back high");
    else if(degY < -pitch) M5.Lcd.print("front high");
  }
  else M5.Lcd.fillRect(0, 60, 320, 40, BLACK);  // over-write previous text
  if (M5.BtnA.wasPressed())
  {
    M5.Speaker.setVolume(0);        // turn off sound
    M5.Lcd.fillRect(240, 10, 40, 40, BLACK);
  }
  if (M5.BtnB.wasPressed())
  {               // turn on sound and
    M5.Speaker.setVolume(80);       //     display speaker image
    M5.Lcd.drawBitmap(240, 10, speaker, 40, 40, WHITE);
  }
  M5.update();
  delay(200);           // delay of 500ms is too jerky
  M5.Lcd.drawCircle(x, y, 8, BLACK);    // over-write circle
  M5.Lcd.drawFastHLine(0, 160, 320, WHITE); // redraw lines after black circle
  M5.Lcd.drawFastVLine(160, 100, 120, WHITE);
  M5.Lcd.setTextColor(TFT_ORANGE, BLACK);
  M5.Lcd.drawString("speaker", 70, 195);
  M5.Lcd.drawString("off      on", 40, 215);  // display speaker labels
  M5.Lcd.setTextColor(GREEN, BLACK);
}
