/*******************************************************************************
 * Sketch name: nRF24L01 as BLE receiver
 * Description: 
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    5 - BLE beacons
 ******************************************************************************/

#include <BTLE.h>            // include BTLE, SPI
#include <SPI.h>            // and RF24 libraries
#include <RF24.h>
RF24 radio(9,10);           // associate radio with RF24
BTLE btle(&radio);          // and btle with BTLE library
String str;
int channel, state;

void setup()
{
  Serial.begin(9600);         // Serial Monitor baud rate
  btle.begin("nRFBLE");         // initialise BTLE device
}               // with name nRFBLE

void loop()
{
  if(btle.listen())       // when BLE advertisement available
  {
    str = "";         // increment string with BLE buffer
    for (int i=2; i<(btle.buffer.pl_size)-6; i++)  
       str = str + btle.buffer.payload[i];
    if(str.length() == 3)       // LED channel, zero, LED state
    {
      Serial.printf("payload %s \n", str);
      channel = str.toInt()/100;      // LED channel
      state = str.toInt() % 10;     // LED state
      pinMode(channel, OUTPUT);     // LED channel as OUTPUT
      digitalWrite(channel, state);     // turn on or off LED
    }
  }
  btle.hopChannel();          // move to next BLE channel
}
