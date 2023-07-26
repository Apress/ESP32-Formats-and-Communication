/*******************************************************************************
 * Sketch name: nRF24L01 as BLE transmitter
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
float temp;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  btle.begin("nRFBLE");         // initialise BTLE device
}               // with name nRFBLE

void loop()
{
  temp = random(1,100);
  nrf_service_data buf;         // define BLE service buffer
  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;  // and UUID
  buf.value = BTLE::to_nRF_Float(temp);   // convert variable to BLE format
  if(!btle.advertise(0x16, &buf, sizeof(buf)))    // check message length
  Serial.println("output error");
  else Serial.println(temp);
  btle.hopChannel();          // move to next BLE channel
  delay(5000);            // delay between transmissions
}
