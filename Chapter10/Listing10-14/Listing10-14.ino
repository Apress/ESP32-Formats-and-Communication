/*******************************************************************************
 * Sketch name: Display image as sprite (2)
 * Description: Display on M5Stack Core2
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <M5Core2.h>          // include M5Core2 library
TFT_eSprite img = TFT_eSprite(&M5.Lcd);   // Sprite image pointer to img
#include "background.h"
#include "background2.h"          // backgrounds in RGB565 and
#include "snowflake.h"          //      foreground in XBM formats
int width = 320, height = 240;      // LCD dimensions
int colorDepth = 16;          // 1, 4, 8 or 16-bit colours
const int Nflake = 250;         // const to define array size
int x[Nflake], y[Nflake];
float speed[Nflake];
float dx, dy;

float accX, accY, accZ;         // accelerometer values
int background = 0, level, volt = 2500;
uint16_t colour;
String text;

void setup()
{
  M5.begin();           // initialise M5Stack module
  M5.Axp.SetLed(0);         // turn off in-built LED
  M5.Axp.SetSpkEnable(0);       // turn off speaker
  M5.IMU.Init();            // initialise accelerometer
  img.setColorDepth(colorDepth);      // Sprite colour depth
  img.createSprite(width, height);      // full screen Sprite
  img.pushImage(0, 0, 320, 240, bckgnd);   // create background image
}

void loop()
{
  if (M5.BtnA.wasPressed()) background = 1-background;
  if(background == 0) img.pushImage(0, 0, 320, 240, bckgnd);
  else img.pushImage(0, 0, 320, 240, bckgnd2);  // change backgrounds
  
  M5.IMU.getAccelData(&accX,&accY,&accZ);   // accelerometer val > threshold
  if(fabs(accY) > 2 || fabs(accX) > 2)    // absolute value for float
  {
    for(int i=0; i < Nflake; i++)
    {
      x[i] = random(width);       // snowflakes horizontally
      y[i] = random(height/2)-height/2-flakeH;  //    distributed and above screen
      speed[i] = random(10,30)/200.0;   // speed for each snowflake
    }
  }
  else
  {
    for (int i=0; i<Nflake; i++)      // move snowflakes
    {               // -accX to correspond to
      dx = -accX*10.0 + accY*random(-5,5);  //    M5 movement
      dy =  accY*10.0 + accY*random(-5,5);  // random(-5, 5) on accY
      x[i] = x[i] + round(dx*speed[i]);
      y[i] = y[i] + round(dy*speed[i]);   // update snowflake position
      constrain(x[i], 0, width-flakeW);   // keep images on screen
      img.drawXBitmap(x[i], y[i], snowflake, flakeW, flakeH, TFT_WHITE);
    }
  }
  img.pushSprite(0, 0);         // display Sprite on LCD screen
  
  if (M5.BtnB.isPressed())
  {
    battLevel();            // call function to display battery
    motor();            // call function to vibrate motor
  }
  if (M5.BtnC.wasPressed()) volt = volt+100;  // increment screen brightness
  if(volt > 3300) volt = 2500;
  M5.Axp.SetLcdVoltage(volt);       // update LCD brightness
  M5.update();            // update button "pressed" status

}

void battLevel()          // function to display battery voltage
{
  level = (M5.Axp.GetBatVoltage() - 3.2)/0.1; // battery level 3.2 - 4.1V
  M5.Lcd.fillRect(160, 100, 160, 140, WHITE); // white rectangle behind graph
  for (int i=0; i<level; i++)       // only display coloured bars
  {
    colour = GREEN;         // battery voltage 3.8-4.1V
    if(i<3) colour = RED;       //    3.2-3.4V
    else if(i<6) colour = YELLOW;     //    3.5-3.7V
    M5.Lcd.fillRoundRect(307,(230-(i*10)),12,7,2,(level>i)
                ? colour : BLACK);      // black above battery voltage bar
    M5.Lcd.drawRoundRect(307,(230-(i*10)),12,7,2,TFT_LIGHTGREY);
  }
  M5.Lcd.setTextColor(BLACK);
  text = "LCD " + String(volt);     // display LCD brightness
  M5.Lcd.drawString(text, 180, 110, 4);
  text = "Level "+String(level);      // drawString cannot build string
  M5.Lcd.drawString(text, 180, 140, 4);
  text = String(M5.Axp.GetBatVoltage())+"V";  // display battery voltage
  M5.Lcd.drawString(text, 180, 170, 4);
  text = String(M5.Axp.GetBatCurrent(),0)+"mA"; // display current usage
  M5.Lcd.drawString(text, 180, 200, 4);   //    with 0DP
}

void motor()            // function for vibration motor
{
  M5.Axp.SetLDOVoltage(3, volt);      // vibration motor voltage
  for (int i=0; i<5; i++)
  {
    M5.Axp.SetLed(1);         // turn on built-in LED
    M5.Axp.SetLDOEnable(3, 1);      // turn on vibration motor
    delay(500);
    M5.Axp.SetLed(0);         // turn off built-in LED
    M5.Axp.SetLDOEnable(3, 0);      // turn off vibration motor
    delay(500);
  }
}
