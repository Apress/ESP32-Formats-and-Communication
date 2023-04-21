/*******************************************************************************
 * Sketch name: Stored data with Preferences
 * Description: Non-volatile memory to store data prior to shutdown
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    1 - ESP32 microcontroller
 ******************************************************************************/

#include <Preferences.h>        // include Preferences library
Preferences pref;           // associate pref with library
int builtin = 2;            // builtin LED on GPIO 2
int btn = 0;            // BOOT button on GPIO 0
int LEDstate = 0;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(btn, INPUT);
  pinMode(builtin, OUTPUT);
  pref.begin("LED");          // namespace called LED
  LEDstate = pref.getShort("state", 0);   // obtain state value from NVS
  digitalWrite(built, LEDstate);      //    with default value of zero
  Serial.printf("LED state before reset %d \n", LEDstate);
}

void loop()
{
  if(digitalRead(btn) == LOW)       // BOOT button pressed
  {
    LEDstate = 1-LEDstate;        // update LED state
    digitalWrite(builtin, LEDstate);
    Serial.printf("LED state %d \n", LEDstate);
    pref.putShort("state", LEDstate);   // update state value in NVS
    delay(500);           // simple debounce
  }
}
