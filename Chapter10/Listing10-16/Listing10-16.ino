/*******************************************************************************
 * Sketch name: Transparent Sprite in paddle game
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <TFT_eSPI.h>          // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();        // associate tft with library
TFT_eSprite back = TFT_eSprite(&tft);   // Sprite for background
TFT_eSprite img = TFT_eSprite(&tft);    //   image Sprite is a ring
TFT_eSprite txt = TFT_eSprite(&tft);    //   Sprites to display text
TFT_eSprite pad = TFT_eSprite(&tft);    //   and the paddle
#include "background.h"         // background in 565 format
int btnRpin = 35, btnLpin = 0;      // right and left buttons
int xDir = 1, x = 0, y = 30;        // direction and position of ring
int dx = 1, dy = 1;         // increments to move ring
int lag = 10;           // speed in x direction
volatile int padFlag = 0, padY = 50;    // paddle position
int score = 0, total = 0, scoreFlag = 0;    // game scores
String scores;

void setup()
{
  tft.init();           // initialise display
  tft.setRotation(1);         // rotation 1 = USB on RHS
  back.createSprite(240, 135);      // background Sprite size
  back.setPivot(0,120);         // rotation point
  img.createSprite(28, 28);       // Sprite to display ring
  img.fillCircle(14, 14, 14, TFT_RED);    // red larger circle
  img.fillCircle(14, 14, 9, TFT_BLACK);   //   and black inner circle
  pad.createSprite(10, 40);       // Sprite to display paddle
  pad.fillRect(0, 0, 10, 40, TFT_GREEN);    //   as a green rectangle
  txt.createSprite(100, 30);        // Sprite to display text
  txt.setTextSize(2);
  txt.setTextColor(TFT_WHITE, TFT_BLACK);   //   red on black
  txt.setPivot(0, 0);         // rotation point
  pinMode(btnRpin,INPUT);
  pinMode(btnLpin,INPUT);     // right button active when released
  attachInterrupt(digitalPinToInterrupt(btnRpin), Yup, RISING);
  attachInterrupt(digitalPinToInterrupt(btnLpin), Ydown, FALLING);
}             // left button active when pressed

IRAM_ATTR void Yup()        // flag that right button was pressed
{
  padFlag = 1;
}
IRAM_ATTR void Ydown()      // move paddle down screen (towards bottom)
{
  padY = padY + 10;
}

void loop()
{
  x = x + xDir * dx;          // increment x position, change
  if(x < 0 || x > 212) xDir = -xDir;    //   direction at end of screen
  y = y + dy;           // change y direction at
  if(y < 0 || y > 107) dy = -dy;      //   top or bottom of screen
  if(x > 212)           // when ring at bottom of screen
  {
    total++;            // increment total number
    if(abs(y - padY - 6) < 15) score++;   // increment score
    if(score % 3 == 0) dx++;        // increment ring Sprite change
  }               //   in the x-co-ordinate
  if(padFlag > 0)           // right button pressed
  {
    padFlag = 0;            // reset paddle flag
    padY = padY - 10;       // move paddle towards top of screen
  }
  scores = String(score) + "/" + String(total);   // text to display scores
  back.setSwapBytes(true);        // required to retain image colour
  back.pushImage(0, 0, 240, 135, backgnd);  // create background image
  back.setSwapBytes(false);       // reset setSwapBytes state
  txt.drawString(scores, 0, 0, 2);      // position within Sprite area
  txt.pushRotated(&back, 270, TFT_BLACK);   // rotate text Sprite 270
  pad.pushToSprite(&back, 230, padY); // paddle Sprite position (230, padY)
  img.pushToSprite(&back, x, y, TFT_BLACK); // ring Sprite position (x, y)
  back.pushSprite(0,0);       // TFT_BLACK is transparent colour
  delay(lag);           // time lag (ms) between displays
}
