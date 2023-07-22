/*******************************************************************************
 * Sketch name: I2C device scanner
 * Description: Display I2C addresses on grid
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    10 - Managing images
 ******************************************************************************/

#include <Wire.h>            // include Wire library
int device, row;
  
void setup()
{
  Wire.begin();           // initialise I2C bus
  Serial.begin(115200);         // Serial Monitor baud rate
}

void loop()
{
  device = 0;           // reset I2C device counter
  row = 1;
  Serial.println("\nI2C Scanner");      // display headers
  Serial.println("    0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
  Serial.print  ("00                        ");
  for (int i=8; i<120; i++)       // scan through channels 8 to 119
  {
    Wire.beginTransmission(i);      // transmit to device at address i
    if(Wire.endTransmission() == 0)     // device response to transmission
    {
      Serial.print(" ");
      if (i < 16) Serial.print("0");
      Serial.print(i, HEX);       // display I2C address in HEX
      device++;
    }
    else Serial.print(" --");       // no device with i2C address
    if ((i + 1) % 16 == 0)        // next row in displayed results
    {
      Serial.printf("\n%d0", row);      // display row header
      row++;
    }
  }
  Serial.printf("\nFound %d device(s) \n", device);  // display scan results
  delay(5000);
}
