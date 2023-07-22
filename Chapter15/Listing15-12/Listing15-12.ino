/*******************************************************************************
 * Sketch name: Library header file
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

#ifndef flashLibrary_h          // note underscore h and not dot h
#define flashLibrary_h
#include <Arduino.h>

class flashLibrary          // same name as library
{
  public:             // functions called by main sketch
    flashLibrary(int pin, int time);
    void begin(int baud = 115200);      // default value for baud rate
    void flashLED();
    const int totalTime = 1000;     // value accessed by main sketch

  private:              // variables local to the library
    int LEDpin;
    int LEDtime;
};                // note semi-colon
#endif              // note #endif at end of header file
