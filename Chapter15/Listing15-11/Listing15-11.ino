/*******************************************************************************
 * Sketch name: Library source code file
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

#include <flashLibrary.h>          // include header file
flashLibrary::flashLibrary(int pin, int time)
{
  LEDpin = pin;
  LEDtime = time;
}

void flashLibrary::begin(int baud)
{                 // equal to public
  Serial.begin(baud);           // definitions
  pinMode(LEDpin, OUTPUT);
}

void flashLibrary::flashLED()
{
  if(Serial.available()) LEDtime = Serial.parseInt();
  digitalWrite(LEDpin, HIGH);
  Serial.printf("HIGH time %d \n", LEDtime);
  delay(LEDtime);
  digitalWrite(LEDpin, LOW);
  delay(totalTime-LEDtime);
}
