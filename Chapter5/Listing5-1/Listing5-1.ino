/*******************************************************************************
 * Sketch name: highByte and lowByte functions
 * Description: Split 2-byte value to high- and low-bytes
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    5 - BLE beacons
 ******************************************************************************/

int val = 0xABCD;            // 16-bit variable
byte high, low;           // 8-bit variables

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  high = highByte(val);         // upper byte
  low = lowByte(val);         // lower byte
  Serial.printf("integers: %d %d \n", high, low);
  high = val >> 8;          // shift right 8 bits
  low = val & 0xFF;         // AND with B11111111
  Serial.printf("integers: %d %d \n", high, low);
}

void loop()             // nothing in loop function
{}
