/*******************************************************************************
 * Sketch name: Sketch including library
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    15 - Libraries
 ******************************************************************************/

#include <flashLibrary.h>        // include library
flashLibrary flash(4, 100);       // associate flash with library
                // pass GPIO pin and LED time
void setup()
{
  flash.begin();            // call library function
}

void loop()
{
  flash.flashLED();         // call library function
  Serial.println(flash.totalTime);      // display library variable
}
