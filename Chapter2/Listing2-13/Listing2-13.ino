/*******************************************************************************
 * Sketch name: Bluetooth and M5Stack Core2
 * Description: Transceive data with Bluetooth
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    2 - I2S Audio
 ******************************************************************************/

#include <M5Core2.h>          // include M5Stack and
#include <BluetoothSerial.h>        //    Bluetooth Serial libraries
BluetoothSerial SerialBT;       // associate SerialBT with library
char c;
String str;
int colour;

void setup()
{
  SerialBT.begin("M5Stack");       // initialise SerialBT
  M5.begin();            //    and M5Stack module
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(4);
}

void loop()
{
  while (SerialBT.available()>0)       // when text received by M5Stack
  {
    c = SerialBT.read();        // read character
    str = str + c;
    if(c == 'g') colour = 0x07E0; // GREEN
    else if(c == 'r')
    {
      colour = 0xF800; // RED
      SerialBT.println("LCD screen now RED");  // send response to connected
    } // Bluetooth app
  }
  if(str.length() > 0)
  {
    M5.Lcd.fillScreen(colour);
    M5.Lcd.setCursor(20,20);
    M5.Lcd.print(str);
    str = "";
    colour = 0x0000;  // BLACK
  }
  delay(100);
}
