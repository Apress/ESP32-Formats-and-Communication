/*******************************************************************************
 * Sketch name: IR remote signal conversion
 * Description: Map LSByte to MSByte orientation
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    4 - TTGO T-Watch V2
 ******************************************************************************/

#include <IRremote.h>          // include IRremote library
int IRpin = 13;           // IR receiver pin
int recv, val[3];
int mapv[] = {0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};    // mapped values

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  IrReceiver.begin(IRpin);        // initialise IR receiver
}

void loop()
{
  if(IrReceiver.decode())       // IR signal received
  {
    recv = IrReceiver.decodedIRData.decodedRawData;      // IR signal
    for (int i=0; i<3; i++) val[i+1] = (recv >> i*4) & 0xF;    // components
    Serial.printf("data %X mapped %X%X%X \n",      // display mapped values
                  recv, mapv[val[1]], mapv[val[2]], mapv[val[3]]);
    delay(200);           // delay before next IR signal
    IrReceiver.resume();
  }
}
