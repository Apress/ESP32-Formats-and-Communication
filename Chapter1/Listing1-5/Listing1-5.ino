/*******************************************************************************
 * Sketch name: Battery base LEDs
 * Description: Flash LEDs on M5Stack Core2 M5G0 battery base
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    1 - ESP32 microcontroller
 ******************************************************************************/
#include <FastLED.h>        // include FastLED library
#define nLEDs 10          // number of LEDs on battery base
CRGB leds[nLEDs];
#define LEDpin 25   // int LEDpin = 25 doesn't function with FastLED library

void setup()
{FastLED.addLeds<NEOPIXEL, LEDpin>(leds, nLEDs);}

void loop()
{
  fill_solid(leds, 10, CRGB::Red);      // turn on LEDs with red colour
  FastLED.show();           // update LED states
  delay(500);
  fill_solid(leds, 10, CRGB::Black);    // turn off LEDs
  FastLED.show();
  delay(500);
}
