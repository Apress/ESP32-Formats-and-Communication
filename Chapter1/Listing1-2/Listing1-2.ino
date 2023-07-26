/*******************************************************************************
 * Sketch name: Battery voltage
 * Description: Display battery votage of TTGO T-Display V1.1 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    1 - ESP32 microcontroller
 ******************************************************************************/

#include <TFT_eSPI.h>          // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();
int enPin = 14, battPin = 34;       // ADC enable and battery pins
int batt1;
unsigned long value, batt2;

void setup()
{
  pinMode(enPin, OUTPUT);       // set ADC enable pin HIGH
  digitalWrite(enPin, HIGH);
  tft.init();
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setRotation(3);         // landscape with USB on left
}
void loop()
{
  value = 0;
  batt2 = 0;
  for (int i=0; i<20; i++)        // take average of 20 readings
  {
    value = value + analogRead(battPin);      // analog value
    batt2 = batt2 + analogReadMilliVolts(battPin);  // read millivolts
  }
  value = value / 20;           // average analog value
  batt1 = round((value * 2.0 * 3.3 * 1100.0)/4095.0); //   converted to millivolts
  batt2 = round(2.0 * batt2 / 20.0);      // average millivolts value
  tft.fillScreen(TFT_BLUE);         // clear screen
  tft.setCursor(0,20);          // display both battery voltages
  tft.printf(" analog %d mV \n\n readmV %d mV \n", batt1, batt2);
  delay(5000);
}
